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

/** @file
 *
 * @defgroup ble_sdk_srv_ias_c Immediate Alert Service Client
 * @{
 * @ingroup ble_sdk_srv
 * @brief Immediate Alert Service Client module
 *
 * @details This module implements the Immediate Alert Service client - locator role of the Find Me
 *          profile. On @ref BLE_GAP_EVT_CONNECTED event, this module starts discovery of the
 *          Immediate Alert Service with Alert Level characteristic at the peer. This module will
 *          indicate the application about a successful service & characteristic discovery using
 *          @ref BLE_IAS_C_EVT_SRV_DISCOVERED event. The application can use @ref
 *          ble_ias_c_send_alert_level function to signal alerts to the peer.
 *
 * @note The application must propagate BLE stack events to this module by calling
 *       ble_ias_c_on_ble_evt() from the @ref softdevice_handler callback function.
 */

#ifndef BLE_IAS_C_H__
#define BLE_IAS_C_H__

#include "ble_srv_common.h"
#include "ble_gattc.h"
#include "ble.h"
#include <stdint.h>

// Forward declaration of the ble_ias_c_t type.
typedef struct ble_ias_c_s ble_ias_c_t;

/**@brief Immediate Alert Service client event type. */
typedef enum
{
    BLE_IAS_C_EVT_SRV_DISCOVERED,                       /**< Event indicating that the Immediate Alert Service is found at the peer. */
    BLE_IAS_C_EVT_SRV_NOT_FOUND,                        /**< Event indicating that the Immediate Alert Service is not found at the peer. */
    BLE_IAS_C_EVT_DISCONN_COMPLETE                      /**< Event indicating that the Immediate Alert Service client module has completed the processing of BLE_GAP_EVT_DISCONNECTED event. This event is raised only if a valid instance of IAS was found at the peer during the discovery phase. This event can be used the application to do clean up related to the IAS Client.*/
} ble_ias_c_evt_type_t;

/**@brief Immediate Alert Service client event. */
typedef struct
{
    ble_ias_c_evt_type_t evt_type;                      /**< Type of event. */
} ble_ias_c_evt_t;

/**@brief Immediate Alert Service client event handler type. */
typedef void (*ble_ias_c_evt_handler_t) (ble_ias_c_t * p_ias_c, ble_ias_c_evt_t * p_evt);

/**@brief IAS Client structure. This contains various status information for the client. */
struct ble_ias_c_s
{
    ble_ias_c_evt_handler_t   evt_handler;              /**< Event handler to be called for handling events in the Immediate Alert Service client. */
    ble_srv_error_handler_t   error_handler;            /**< Function to be called in case of an error. */
    uint16_t                  alert_level_handle;       /**< Handle of Alert Level characteristic at peer (as provided by the BLE stack). */
    uint16_t                  conn_handle;              /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
};

/**@brief IAS Client init structure. This contains all options and data needed for initialization of
 *        the client.*/
typedef struct
{
    ble_ias_c_evt_handler_t   evt_handler;              /**< Event handler to be called for handling events from the Immediate Alert Service client. */
    ble_srv_error_handler_t   error_handler;            /**< Function to be called in case of an error. */
} ble_ias_c_init_t;

/**@brief Function for initializing the Immediate Alert Service client.
 *
 * @details This call allows the application to initialize the Immediate Alert Service client.
 *
 * @param[out]  p_ias_c      Immediate Alert Service client structure. This structure will have to
 *                           be supplied by the application. It will be initialized by this
 *                           function, and will later be used to identify this particular client
 *                           instance.
 * @param[in]   p_ias_c_init Information needed to initialize the Immediate Alert Service client.
 *
 * @return      NRF_SUCCESS on successful initialization of service.
 */
uint32_t ble_ias_c_init(ble_ias_c_t * p_ias_c, const ble_ias_c_init_t * p_ias_c_init);

/**@brief Function for sending alert level to the peer.
 *
 * @details This function allows the application to send an alert to the peer.
 *
 * @param[in]   p_ias_c      Immediate Alert Service client structure.
 * @param[in]   alert_level  Required alert level to be sent to the peer.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_ias_c_send_alert_level(const ble_ias_c_t * p_ias_c, uint8_t alert_level);

/**@brief Function for handling the Application's BLE Stack events for Immediate Alert Service client.
 *
 * @details Handles all events from the BLE stack of interest to the Immediate Alert Service client.
 *
 * @param[in]   p_ias_c      Immediate Alert Service client structure.
 * @param[in]   p_ble_evt    Event received from the BLE stack.
 */
void ble_ias_c_on_ble_evt(ble_ias_c_t * p_ias_c, const ble_evt_t * p_ble_evt);

/**@brief Function for checking whether the peer's Immediate Alert Service instance and the alert level
 *        characteristic have been discovered.
 * @param[in]  p_ias_c      Immediate Alert Service client structure.
 */
static __INLINE bool ble_ias_c_is_ias_discovered(const ble_ias_c_t * p_ias_c)
{
    return (p_ias_c->alert_level_handle != BLE_GATT_HANDLE_INVALID);
}

#endif

/** @} */
