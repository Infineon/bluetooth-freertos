/*******************************************************************************
* \file cybt_platform_config.h
*
* \brief
* Provides interface to configurate platform settings, including HCI, sleep mode
* and OS task memory pool.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef CYBT_PLATFORM_CONFIG_H
#define CYBT_PLATFORM_CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "cyhal_gpio.h"
#include "cyhal_uart.h"


/******************************************************************************
 *                                Constants
 ******************************************************************************/
#define CYBT_SLEEP_MODE_DISABLED      (0)
#define CYBT_SLEEP_MODE_ENABLED       (1)

#define CYBT_WAKE_ACTIVE_LOW          (0)
#define CYBT_WAKE_ACTIVE_HIGH         (1)


/*****************************************************************************
 *                           Type Definitions
 *****************************************************************************/
/*
 *  The BT HCI transport type
 */
typedef enum
{
    CYBT_HCI_UNKNOWN = 0x00,
    CYBT_HCI_UART    = 0x01,
    CYBT_HCI_USB     = 0x02
} cybt_hci_transport_t;

/*
 *  The HCI UART configuration, including:
 *  1) hardware pin assignment
 *  2) baud rate
 *  3) data format
 *  4) flow control support
 */
typedef struct
{
    cyhal_gpio_t         uart_tx_pin;
    cyhal_gpio_t         uart_rx_pin;
    cyhal_gpio_t         uart_rts_pin;
    cyhal_gpio_t         uart_cts_pin;

    /*
     *  The UART baud rate for downloading firmware
     *  during Cypress Bluetooth chip initialization.
     */
    uint32_t             baud_rate_for_fw_download;

    /*
     *  The UART baud rate for Bluetooth/BLE feature
     *  after Cypress Bluetooth chip initialization.
     */
    uint32_t             baud_rate_for_feature;

    uint32_t             data_bits;
    uint32_t             stop_bits;
    cyhal_uart_parity_t  parity;
    bool                 flow_control;
} cybt_hci_uart_config_t;

/*
 *  The configuration of BT HCI transport, to specify which interface
 *  is used and its format.
 *
 *  Currently only UART is supported.
 */
typedef struct
{
    cybt_hci_transport_t  hci_transport;

    union
    {
        cybt_hci_uart_config_t  hci_uart;
    } hci;
} cybt_hci_transport_config_t;

/*
 *  The BT chip sleep mode configuration
 */
typedef struct
{
    /*
     * Enable or disable the sleep mode of BT chip
     *
     *  Either assign the value
     *      CYBT_SLEEP_MODE_DISABLED
     *      CYBT_SLEEP_MODE_ENABLED
     *
     *  or CYCFG_BT_LP_ENABLED for ModusToolBox LPA configuration
     *
     *  Notice that sleep mode is enabled only if both device wakeup
     *  and host wakeup pins are assigned. 
     */
    uint8_t               sleep_mode_enabled;

    /*
     *  Provide the gpio definition for BT device wakeup pin,
     *
     *  It can be assigned by the value with the type cyhal_gpio_t directly
     *  For ModusToolBox, CYCFG_BT_DEV_WAKE_GPIO can be used. 
     *  
     *  NC, is used as this pin is NOT connected
     *
     */
    cyhal_gpio_t          device_wakeup_pin;

    /*
     *  Provide the pin definition for BT host wakeup pin,
     *
     *  It can be assigned by the value with the type cyhal_gpio_t directly
     *  For ModusToolBox, CYCFG_BT_HOST_WAKE_GPIO can be used. 
     *  
     *  NC, is used as this pin is NOT connected
     *
     */
    cyhal_gpio_t          host_wakeup_pin;

    /*
     *  Provide the trigger level of BT device wakeup pin
     *
     *  Either assign the value
     *      CYBT_WAKE_ACTIVE_LOW
     *      CYBT_WAKE_ACTIVE_HIGH
     *
     *  or CYCFG_BT_DEV_WAKE_POLARITY for ModusToolBox LPA configuration
     *
     *  The default value will be active low.
     */
    uint8_t               device_wake_polarity;

    /*
     *  Provide the trigger level of BT host wakeup pin
     *
     *  Either assign the value
     *      CYBT_WAKE_ACTIVE_LOW
     *      CYBT_WAKE_ACTIVE_HIGH
     *
     *  or CYCFG_BT_HOST_WAKE_IRQ_EVENT for ModusToolBox LPA configuration
     *
     *  The default value will be active low.
     */
    uint8_t               host_wake_polarity;
} cybt_controller_sleep_config_t;

/*
 *  The BT chip related configuration
 */
typedef struct
{
    cyhal_gpio_t                    bt_power_pin;
    cybt_controller_sleep_config_t  sleep_mode;
} cybt_controller_config_t;

/*
 *  The overall configuration for Cypress WICED BT/BLE stack and BT chip
 */
typedef struct
{
    /*
     *  Bluetooth HCI transport configuration,
     *  including UART pins and baud rate.
     */
    cybt_hci_transport_config_t    hci_config;

    /*
     *  Cypress Bluetooth chip control pins
     */
    cybt_controller_config_t       controller_config;

    /*
     *  The maximum size of memory pool which is used for
     *  Bluetooth task communication.
     *  The default size is 2048 bytes if it wasn't specified.
     */
    uint32_t                       task_mem_pool_size;

} cybt_platform_config_t;


#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 *                           Function Declarations
 ****************************************************************************/

/** This function is used to configurate the platform specific settings.
 *  1) HCI transport
 *  2) Controller settings, including sleep mode
 *  3) The size of OS task memory pool
 *
 *  For detail please check all above structures.
 *
 * @returns
 */
void cybt_platform_config_init(const cybt_platform_config_t *p_bt_platform_cfg);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

