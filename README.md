# WICED Bluetooth Host Stack solution (for FreeRTOS)

## Overview
WICED Bluetooth host stack solution includes bluetooth stack library,
bluetooth controller firmware and platform/os porting layer. Bluetooth stack library
is designed for embedded device, it consumes less RAM/ROM usage but still keeps
high performance. With WICED Bluetooth API set, application developers can use them
easily to create their own application. The porting layer is implemented by CYHAL and CY_RTOS_AL
(Hardware/Operation System Adaptation Layer), hence it can adapt to Cypress platforms, and easy to 
port to other vendor's platform.  

## Platform HCI transport config
Besides of stack settings, HCI configuration is also required to specify, 
including pins, format and UART baudrate. Please refer to cybt_platform_config.h 
for more detail:

 - structure **cybt_platform_config_t**
 - API **cybt_platform_config_init( )**

The API **cybt_platform_config_init( )** shall be invoked prior to 
**wiced_bt_stack_init( )**

## How to enable trace log?
 - *Compile time definition:* Please refer to cybt_platform_trace.h which can set individual trace log level (Default set all category is CYBT_TRACE_LEVEL_ERROR)
 - *Run time update:* Dynamic set trace level by using API **cybt_platform_set_trace_level(id, level);**
   - For example: set all catoegories as debug level
	   ```
	   cybt_platform_set_trace_level(CYBT_TRACE_ID_ALL, CYBT_TRACE_LEVEL_DEBUG);
	   ```
 - Undefine the CYBT_PLATFORM_TRACE_ENABLE macro to disable all debug log messages

## How to enable BTSpy logs?
 - BTSpy is a trace utility that can be used in the WICED BT platforms to view protocol and generic trace messages from the embedded device
 - Add macro ENABLE_BT_SPY_LOG in Makefile or command line
   - `DEFINES+=ENABLE_BT_SPY_LOG`
 - Call **cybt_debug_uart_init(&debug_uart_configuration, NULL);**
   - The first argument is the debug_uart_configurations structure pointer, which has hardware pin configurations along with baud_rate and flow_control configurations. Recommended baudrate is 3000000, although 115200 is also supported by the BTSpy tool. The second argument is an optional callback function which can be set to NULL
   - Ensure retarget-io is not enabled on the same UART port as BTSpy. There is no need to initialize the retarget-io libray if the application wants to send both application messages and BT protocol traces to the same port through BTSpy
   - Compiler directives can be used to either initialize the retarget-io library or BTSpy logs depending on the debug macro setting. For example:
     ```
     #ifdef ENABLE_BT_SPY_LOG
        {
            #define DEBUG_UART_BAUDRATE 3000000
            #define DEBUG_UART_RTS        (P5_2)
            #define DEBUG_UART_CTS        (P5_3)
            cybt_debug_uart_config_t debug_uart_config = {
                    .uart_tx_pin = CYBSP_DEBUG_UART_TX,
                    .uart_rx_pin = CYBSP_DEBUG_UART_RX,
                    .uart_cts_pin = DEBUG_UART_CTS,
                    .uart_rts_pin = DEBUG_UART_RTS,
                    .baud_rate = DEBUG_UART_BAUDRATE,
                    .flow_control = TRUE
            };
            cybt_debug_uart_init(&debug_uart_config, NULL);
        }
     #else
        cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
     #endif
     ```
 - Download and use [BTSPY](https://github.com/Infineon/btsdk-utils)
   - Click on serial port setup
   - Select "Enable Serial Port"
   - Select the corrct baudrate, port number and enable HW flow control

## Working flow

### ~ release-v1.3.0
 - Porting layer create 2 tasks
   - **HCI task** which handles HCI packet transmission and reception
   - **BTU/BT task** which handles Bluetooth core stack and profiles
 - TX path:
   - BT stack calls pf_write_xxx_to_lower( ) with packet data
   - Inside those functions,
       - Allocate memory from porting layer heap for the TX packet
       - Put TX packet into HCI task queue
   - HCI task gets TX packet from the queue then write the same to HCI UART
 - RX path:
   - UART driver fires the interrupt when data was coming from BT controller
   - In IRQ handler, put the related message to HCI task queue
   - Once HCI task gets the message,
     - Allocate memory from porting layer heap, read the packet from UART and write the same to the allocated memory
     - Put RX packet into BT task queue.
   - BT task gets RX packet then calls wiced_bt_process_xxx() to notify BT stack

### since release-v2.0.0
 - Porting layer create 2 tasks
   - **HCI_TX task** which handles HCI packet from BT stack to BT controller
   - **HCI_RX task** which handles HCI packet from BT controller to BT stack
 - TX path:
   - BT stack calls pf_write_xxx_to_lower( ) with packet data
   - Inside those functions,
     - Allocate memory from porting layer heap for the TX packet
     - Put TX packet into HCI_TX task queue
   - HCI_TX task gets TX packet from the queue then write the same to HCI UART
 - RX path:
   - UART driver fires the interrupt when data was coming from BT controller
   - In IRQ handler, put the related message to HCI_RX task queue
   - Once HCI_RX task gets the message,
     - Read the packet from UART and write the same to the static buffer
     - Call wiced_bt_process_xxx() to notify BT stack

## API Reference Manual
 - [Bluetooth platform API manual](https://cypresssemiconductorco.github.io/bluetooth-freertos/api_reference_manual/html/index.html)
 - [Bluetooth stack BLE API manual](https://cypresssemiconductorco.github.io/btstack/ble/api_reference_manual/html/index.html)
    
© Cypress Semiconductor Corporation, 2020.