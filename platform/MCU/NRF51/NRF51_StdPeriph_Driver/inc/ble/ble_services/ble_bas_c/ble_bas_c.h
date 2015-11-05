/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**@file
 *
 * @defgroup ble_sdk_srv_bas_c Battery Service Client
 * @{
 * @ingroup  ble_sdk_srv
 * @brief    Battery Service Client module.
 *
 * @details  This module contains APIs to read and interact with the Battery Service of a remote
 *           device.
 *
 * @note     The application must propagate BLE stack events to this module by calling
 *           ble_hrs_c_on_ble_evt().
 *
 */

#ifndef BLE_BAS_C_H__
#define BLE_BAS_C_H__

#include <stdint.h>
#include "ble.h"

/**
 * @defgroup bas_c_enums Enumerations
 * @{
 */

/**@brief Battery Service Client event type. */
typedef enum
{
    BLE_BAS_C_EVT_DISCOVERY_COMPLETE,  /**< Event indicating that the Battery Service has been discovered at the peer. */
    BLE_BAS_C_EVT_BATT_NOTIFICATION,   /**< Event indicating that a notification of the Battery Level characteristic has been received from the peer. */
    BLE_BAS_C_EVT_BATT_READ_RESP       /**< Event indicating that a read response on Battery Level characteristic has been received from peer. */
} ble_bas_c_evt_type_t;

/** @} */

/**
 * @defgroup bas_c_structs Structures
 * @{
 */

/**@brief Battery Service Client Event structure. */
typedef struct
{
    ble_bas_c_evt_type_t evt_type;  /**< Event Type. */
    union
    {
        uint8_t battery_level;  /**< Battery level received from peer. This field will be used for the events @ref BLE_BAS_C_EVT_BATT_NOTIFICATION and @ref BLE_BAS_C_EVT_BATT_READ_RESP.*/
    } params;
} ble_bas_c_evt_t;

/** @} */

/**
 * @defgroup bas_c_types Types
 * @{
 */

// Forward declaration of the ble_bas_t type.
typedef struct ble_bas_c_s ble_bas_c_t;

/**@brief   Event handler type.
 *
 * @details This is the type of the event handler that should be provided by the application
 *          of this module in order to receive events.
 */
typedef void (* ble_bas_c_evt_handler_t) (ble_bas_c_t * p_bas_bas_c, ble_bas_c_evt_t * p_evt);

/** @} */

/**
 * @addtogroup bas_c_structs
 * @{
 */

/**@brief      Battery Service Client structure.

 */
struct ble_bas_c_s
{
    uint16_t                conn_handle;     /**< Connection handle as provided by the SoftDevice. */
    uint16_t                bl_cccd_handle;  /**< Handle of the CCCD of the Battery Level characteristic. */
    uint16_t                bl_handle;       /**< Handle of the Battery Level characteristic as provided by the SoftDevice. */
    ble_bas_c_evt_handler_t evt_handler;     /**< Application event handler to be called when there is an event related to the Battery service. */
};

/**@brief   Battery Service Client initialization structure.
 */
typedef struct
{
    ble_bas_c_evt_handler_t evt_handler;  /**< Event handler to be called by the Battery Service Client module whenever there is an event related to the Battery Service. */
} ble_bas_c_init_t;

/** @} */

/**
 * @defgroup bas_c_functions Functions
 * @{
 */

/**@brief      Function for initializing the Battery Service Client module.
 *
 * @details    This function will initialize the module and set up Database Discovery to discover
 *             the Battery Service. After calling this function, call @ref ble_db_discovery_start
 *             to start discovery.
 *
 * @param[out] p_ble_bas_c      Pointer to the Battery Service client structure.
 * @param[in]  p_ble_bas_c_init Pointer to the Battery Service initialization structure containing
 *                              the initialization information.
 *
 * @retval     NRF_SUCCESS      Operation success.
 * @retval     NRF_ERROR_NULL   A parameter is NULL.
 *                              Otherwise, an error code returned by @ref ble_db_discovery_evt_register.
 */
uint32_t ble_bas_c_init(ble_bas_c_t * p_ble_bas_c, ble_bas_c_init_t * p_ble_bas_c_init);


/**@brief     Function for handling BLE events from the SoftDevice.
 *
 * @details   This function will handle the BLE events received from the SoftDevice. If the BLE
 *            event is relevant for the Battery Service Client module, then it is used to update
 *            interval variables and, if necessary, send events to the application.
 *
 * @note      This function must be called by the application.
 *
 * @param[in] p_ble_bas_c Pointer to the Battery Service client structure.
 * @param[in] p_ble_evt   Pointer to the BLE event.
 */
void ble_bas_c_on_ble_evt(ble_bas_c_t * p_ble_bas_c, const ble_evt_t * p_ble_evt);


/**@brief   Function for enabling notifications on the Battery Level characteristic.
 *
 * @details This function will enable to notification of the Battery Level characteristic at the
 *          peer by writing to the CCCD of the Battery Level Characteristic.
 *
 * @param   p_ble_bas_c Pointer to the Battery Service client structure.
 *
 * @retval  NRF_SUCCESS     If the SoftDevice has been requested to write to the CCCD of the peer.
 *          NRF_ERROR_NULL  Parameter is NULL.
 *                          Otherwise, an error code returned by the SoftDevice API @ref 
 *                          sd_ble_gattc_write.
 */
uint32_t ble_bas_c_bl_notif_enable(ble_bas_c_t * p_ble_bas_c);


/**@brief   Function for reading the Battery Level characteristic.
 *
 * @param   p_ble_bas_c Pointer to the Battery Service client structure.
 *
 * @retval  NRF_SUCCESS If the read request was successfully queued to be sent to peer.
 */
uint32_t ble_bas_c_bl_read(ble_bas_c_t * p_ble_bas_c);


/** @} */ // End tag for Function group.

#endif // BLE_BAS_C_H__

/** @} */ // End tag for the file.
