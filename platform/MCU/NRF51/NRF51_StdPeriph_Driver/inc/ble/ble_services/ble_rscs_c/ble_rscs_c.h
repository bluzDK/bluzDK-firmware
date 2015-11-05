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

/**@brief RSCS Client event type. */
typedef enum
{
    BLE_RSCS_C_EVT_DISCOVERY_COMPLETE = 1,  /**< Event indicating that the Running Speed and Cadence Service has been discovered at the peer. */
    BLE_RSCS_C_EVT_RSC_NOTIFICATION         /**< Event indicating that a notification of the Running Speed and Cadence measurement characteristic has been received from the peer. */
} ble_rscs_c_evt_type_t;

#define BLE_RSCS_INSTANT_STRIDE_LEN_PRESENT    0x00  /**< Instantaneous Stride Length Measurement Supported bit. */
#define BLE_RSCS_TOTAL_DISTANCE_PRESENT        0x01  /**< Total Distance Measurement Supported bit. */
#define BLE_RSCS_WALKING_OR_RUNNING_STATUS_BIT 0x02  /**< Walking or Running Status Supported bit. */

/**@brief Structure containing the Running Speed and Cadence measurement received from the peer. */
typedef struct
{
    bool     is_inst_stride_len_present;             /**< True if Instantaneous Stride Length is present in the measurement. */
    bool     is_total_distance_present;              /**< True if Total Distance is present in the measurement. */
    bool     is_running;                             /**< True if running, False if walking. */
    uint16_t inst_speed;                             /**< Instantaneous Speed. */
    uint8_t  inst_cadence;                           /**< Instantaneous Cadence. */
    uint16_t inst_stride_length;                     /**< Instantaneous Stride Length. */
    uint32_t total_distance;                         /**< Total Distance. */
} ble_rsc_t;

/**@brief Running Speed and Cadence Event structure. */
typedef struct
{
    ble_rscs_c_evt_type_t evt_type;  /**< Type of the event. */
    union
    {
        ble_rsc_t rsc;  /**< Running Speed and Cadence measurement received. This will be filled if the evt_type is @ref BLE_RSCS_C_EVT_RSC_NOTIFICATION. */
    } params;
} ble_rscs_c_evt_t;


// Forward declaration of the ble_rscs_c_t type.
typedef struct ble_rscs_c_s ble_rscs_c_t;

/**@brief   Event handler type.
 *
 * @details This is the type of the event handler that should be provided by the application
 *          of this module in order to receive events.
 */
typedef void (* ble_rscs_c_evt_handler_t) (ble_rscs_c_t * p_ble_rscs_c, ble_rscs_c_evt_t * p_evt);

/**@brief Running Speed and Cadence client structure.
 */
struct ble_rscs_c_s
{
    uint16_t                 conn_handle;      /**< Connection handle as provided by the SoftDevice. */
    uint16_t                 rsc_cccd_handle;  /**< Handle of the CCCD of the Running Speed and Cadence characteristic. */
    uint16_t                 rsc_handle;       /**< Handle of the Running Speed and Cadence characteristic as provided by the SoftDevice. */
    ble_rscs_c_evt_handler_t evt_handler;      /**< Application event handler to be called when there is an event related to the Running Speed and Cadence service. */
};

/**@brief Running Speed and Cadence client initialization structure.
 */
typedef struct
{
    ble_rscs_c_evt_handler_t evt_handler;  /**< Event handler to be called by the Running Speed and Cadence Client module whenever there is an event related to the Running Speed and Cadence Service. */
} ble_rscs_c_init_t;

uint32_t ble_rscs_c_init(ble_rscs_c_t * p_ble_rscs_c, ble_rscs_c_init_t * p_ble_rscs_c_init);

void ble_rscs_c_on_ble_evt(ble_rscs_c_t * p_ble_rscs_c, const ble_evt_t * p_ble_evt);

uint32_t ble_rscs_c_rsc_notif_enable(ble_rscs_c_t * p_ble_rscs_c);


#endif // BLE_RSCS_C_H__

/** @} */ // End tag for the file.
