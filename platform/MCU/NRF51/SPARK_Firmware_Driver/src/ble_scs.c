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

#include "ble_scs.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_util.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

bool waitForTxComplete = true;

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_scs       LED Button Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(scs_t * p_scs, ble_evt_t * p_ble_evt)
{
	p_scs->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_scs       LED Button Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(scs_t * p_scs, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_scs->conn_handle = BLE_CONN_HANDLE_INVALID;
}

volatile uint16_t readBufferLength = 0;
uint8_t readBuffer[1024];
/**@brief Function for handling the Write event.
 *
 * @param[in]   p_scs       LED Button Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(scs_t * p_scs, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if ((p_evt_write->handle == p_scs->data_dn_handles.value_handle) &&
        (p_scs->data_write_handler != NULL))
    {
    	if (p_evt_write->len == 2 && p_evt_write->data[0] == 0x03 && p_evt_write->data[1] == 0x04) {
    		p_scs->data_write_handler(p_scs, (uint8_t*)&readBuffer, readBufferLength);
    		readBufferLength = 0;
    	} else {
//    		for (int i = 0; i < p_evt_write->len; i++) {
//    			readBuffer[readBufferLength+i] = p_evt_write->data[i];
//    		}
            memcpy(readBuffer+readBufferLength, p_evt_write->data, p_evt_write->len);
            readBufferLength += p_evt_write->len;
    	}
    }
}


void scs_on_ble_evt(scs_t * p_scs, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_scs, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_scs, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_scs, p_ble_evt);
            break;

        case BLE_EVT_TX_COMPLETE:
        	waitForTxComplete = false;
			break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for adding the LED characteristic.
 *
 */
static uint32_t data_dn_add(scs_t * p_scs, const scs_init_t * p_scs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.write_wo_resp = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_scs->uuid_type;
    ble_uuid.uuid = BLE_SCS_UUID_DATA_DN_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = 20;
    attr_char_value.p_value      = NULL;

    return sd_ble_gatts_characteristic_add(p_scs->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_scs->data_dn_handles);
}

/**@brief Function for adding the Button characteristic.
 *
 */
static uint32_t data_up_add(scs_t * p_scs, const scs_init_t * p_scs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_scs->uuid_type;
    ble_uuid.uuid = BLE_SCS_UUID_DATA_UP_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = 20;
    attr_char_value.p_value      = NULL;

    return sd_ble_gatts_characteristic_add(p_scs->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_scs->data_up_handles);
}

uint32_t scs_init(scs_t * p_scs, const scs_init_t * p_scs_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_scs->conn_handle       = BLE_CONN_HANDLE_INVALID;
    p_scs->data_write_handler = p_scs_init->data_write_handler;

    // Add service
    ble_uuid128_t base_uuid = BLE_SCS_UUID_BASE;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_scs->uuid_type);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    ble_uuid.type = p_scs->uuid_type;
    ble_uuid.uuid = BLE_SCS_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_scs->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = data_up_add(p_scs, p_scs_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = data_dn_add(p_scs, p_scs_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}

uint32_t scs_data_send(scs_t * p_scs, uint8_t *data, uint16_t len)
{
    ble_gatts_hvx_params_t params;
    //uint16_t len = sizeof(data);
    int error = NRF_SUCCESS;
    uint8_t buffer[20];

    for (int i = 0; i < len; i+=20) {
    	uint16_t size = (len-i > 20 ? 20 : len-i);
    	memcpy(buffer, data+i, size);

		memset(&params, 0, sizeof(params));
		params.type = BLE_GATT_HVX_NOTIFICATION;
		params.handle = p_scs->data_up_handles.value_handle;
		params.p_data = buffer;
		params.p_len = &size;

		int error = sd_ble_gatts_hvx(p_scs->conn_handle, &params);

		//when sending data very fast, we can use up all the buffers. this should protect us as we wait for the TX complete event
		while (error == BLE_ERROR_NO_TX_BUFFERS) {
			while (waitForTxComplete) {
				uint8_t bufferCount = 0;
				sd_ble_tx_buffer_count_get(&bufferCount);
                //TO DO: This can probably be reduced...
				nrf_delay_us(500000);
			}
			waitForTxComplete = true;
			error = sd_ble_gatts_hvx(p_scs->conn_handle, &params);
		}


		if (error != NRF_SUCCESS) {
			return error;
		}
    }

    //send the end of transmission characters
    uint8_t eotBuffer[2] = {3, 4};
    uint16_t eotSize = sizeof(eotBuffer);
    memset(&params, 0, sizeof(params));
	params.type = BLE_GATT_HVX_NOTIFICATION;
	params.handle = p_scs->data_up_handles.value_handle;
	params.p_data = eotBuffer;
	params.p_len = &eotSize;
	error = sd_ble_gatts_hvx(p_scs->conn_handle, &params);

	while (error == BLE_ERROR_NO_TX_BUFFERS) {
		while (waitForTxComplete) {
			nrf_delay_us(500);
		}
		waitForTxComplete = true;
		error = sd_ble_gatts_hvx(p_scs->conn_handle, &params);
	}
    return error;
}
