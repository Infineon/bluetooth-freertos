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

*  structure **cybt_platform_config_t**
*  API **cybt_platform_config_init( )**
  
The API **cybt_platform_config_init( )** shall be invoked prior to 
**wiced_bt_stack_init( )**

## API Reference Manual
 - [Bluetooth platform API manual](https://cypresssemiconductorco.github.io/bluetooth-freertos/api_reference_manual/html/index.html)
 - [Bluetooth stack API manual](https://cypresssemiconductorco.github.io/btstack/api_reference_manual/html/index.html)
    
© Cypress Semiconductor Corporation, 2020.