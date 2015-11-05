/**
 Copyright (c) 2015 MidAir Technology, LLC.  All rights reserved.
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BLE_SCS_H__
#define BLE_SCS_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define BLE_SCS_UUID_BASE {{0xB2, 0x2D, 0x14, 0xAA, 0xB3, 0x9F, 0x41, 0xED, 0xB1, 0x77, 0xFF, 0x38, 0xD8, 0x17, 0x1E, 0x87}};
#define BLE_SCS_UUID_SERVICE 0x0223
#define BLE_SCS_UUID_DATA_DN_CHAR 0x0225
#define BLE_SCS_UUID_DATA_UP_CHAR 0x0224

// Forward declaration of the bcs_t type.
typedef struct scs_s scs_t;

typedef void (*scs_data_write_handler_t) (scs_t * p_scs, uint8_t *data, uint16_t length);

typedef struct
{
    scs_data_write_handler_t data_write_handler;                    /**< Event handler to be called when data is written to characteristic. */
} scs_init_t;

/**@brief SparkLE Communication Service. This contains various status information for the service. */
typedef struct scs_s
{
    uint16_t                    service_handle;
    ble_gatts_char_handles_t    data_up_handles;
    ble_gatts_char_handles_t    data_dn_handles;
    uint8_t                     uuid_type;
    uint16_t                    conn_handle;
    scs_data_write_handler_t 	data_write_handler;
} scs_t;

/**@brief Function for initializing the Banc Custom Service.
 *
 * @param[out]  p_scs       LED Button Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_scs_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t scs_init(scs_t * p_scs, const scs_init_t * p_scs_init);

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the LED Button Service.
 *
 *
 * @param[in]   p_scs      LED Button Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void scs_on_ble_evt(scs_t * p_scs, ble_evt_t * p_ble_evt);

/**@brief Function for sending a data chunk
 */
uint32_t scs_data_send(scs_t * p_scs, uint8_t* data, uint16_t len);

#endif // BLE_SCS_H__

/** @} */
