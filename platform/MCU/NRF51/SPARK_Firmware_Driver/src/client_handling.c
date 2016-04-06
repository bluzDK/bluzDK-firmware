/*
 * Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */

#include "client_handling.h"
#include <string.h>
#include <stdbool.h>
#include "nrf.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "app_trace.h"
#include "ble_db_discovery.h"
#include "ble_srv_common.h"
#include "ble_hci.h"
#include "nrf_delay.h"
#include "spi_slave_stream.h"
#include "app_uart.h"
#include "registered_data_services.h"

#include "debug.h"

#define MULTILINK_PERIPHERAL_BASE_UUID {{0xB2, 0x2D, 0x14, 0xAA, 0xB3, 0x9F, 0x41, 0xED, 0xB1, 0x77, 0xFF, 0x38, 0xD8, 0x17, 0x1E, 0x87}};
#define BLE_SCS_UUID_SERVICE 0x0223
#define BLE_SCS_UUID_DATA_DN_CHAR 0x0224
#define BLE_SCS_UUID_DATA_UP_CHAR 0x0225

#define RX_BUFFER_SIZE 					  512

bool waitForTxComplete = true;

/**@brief Client states. */
typedef enum
{
    IDLE,                                           /**< Idle state. */
    STATE_SERVICE_DISC,                             /**< Service discovery state. */
    STATE_NOTIF_ENABLE,                             /**< State where the request to enable notifications is sent to the peer. . */
    STATE_RUNNING,                                  /**< Running state. */
    STATE_ERROR                                     /**< Error state. */
} client_state_t;

/**@brief Client context information. */
typedef struct
{
    ble_db_discovery_t           srv_db;            /**< The DB Discovery module instance associated with this client. */
    dm_handle_t                  handle;            /**< Device manager identifier for the device. */
    uint8_t                      up_char_index;        /**< Client characteristics index in discovered service information. */
    uint8_t                      dn_char_index;        /**< Client characteristics index in discovered service information. */
    uint8_t                      state;             /**< Client state. */
    uint8_t                      ble_read_buffer[RX_BUFFER_SIZE];
    uint16_t                     ble_read_buffer_length;
    uint8_t                      id;
    bool                         socketedParticle;
    bool                         peripheralConnected;
} client_t;

static client_t         m_client[MAX_CLIENTS];      /**< Client context information list. */
static uint8_t          m_client_count;             /**< Number of clients. */
static uint8_t          m_base_uuid_type;           /**< UUID type. */

static void blink_led(int count)
{
	for (int i = 0; i < count; i++) {
		nrf_gpio_pin_set(GATEWAY_NOTIFICATION_LED);
		nrf_delay_us(100000);
		nrf_gpio_pin_clear(GATEWAY_NOTIFICATION_LED);
		nrf_delay_us(100000);
	}
	nrf_delay_us(300000);
}

/**@brief Function for finding client context information based on handle.
 *
 * @param[in] conn_handle  Connection handle.
 *
 * @return client context information or NULL upon failure.
 */
static uint32_t client_find(uint16_t conn_handle)
{
    uint32_t i;

    for (i = 0; i < MAX_CLIENTS; i++)
    {
        if (m_client[i].srv_db.conn_handle == conn_handle)
        {
            return i;
        }
    }

    return MAX_CLIENTS;
}


/**@brief Function for service discovery.
 *
 * @param[in] p_client Client context information.
 */
static void service_discover(client_t * p_client)
{
    uint32_t   err_code;

    p_client->state = STATE_SERVICE_DISC;

    err_code = ble_db_discovery_start(&(p_client->srv_db),
                                      p_client->srv_db.conn_handle);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling enabling notifications.
 *
 * @param[in] p_client Client context information.
 */
static void notif_enable(client_t * p_client)
{
    uint32_t                 err_code;
    ble_gattc_write_params_t write_params;
    uint8_t                  buf[BLE_CCCD_VALUE_LEN];

    p_client->state = STATE_NOTIF_ENABLE;

    buf[0] = BLE_GATT_HVX_NOTIFICATION;
    buf[1] = 0;

    write_params.write_op = BLE_GATT_OP_WRITE_REQ;
    write_params.handle   = p_client->srv_db.services[0].charateristics[p_client->dn_char_index].cccd_handle;
    write_params.offset   = 0;
    write_params.len      = sizeof(buf);
    write_params.p_value  = buf;
    
    err_code = sd_ble_gattc_write(p_client->srv_db.conn_handle, &write_params);
    APP_ERROR_CHECK(err_code);
}

void spi_slave_set_tx_buffer(client_t * p_client, gateway_function_t type, uint8_t * data, uint16_t len)
{
    data[0] = (( (len-SPI_HEADER_SIZE-BLE_HEADER_SIZE) & 0xFF00) >> 8);
    data[1] = ( (len-SPI_HEADER_SIZE-BLE_HEADER_SIZE) & 0xFF);
    data[2] = p_client->id;
	tx_callback(data, len);
}

/**@brief Funtion for sending data to the client
 *
 * @param[in] data  Data to be sent to the client.
 * @param[in] len   Lengthof the data to be sent to the client.
 */
void on_write(client_t * p_client, ble_evt_t * p_ble_evt)
{
	ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

	for (int i = 0; i < p_evt_write->len; i++) {
        p_client->ble_read_buffer[p_client->ble_read_buffer_length+i] = p_evt_write->data[i];
	}
    p_client->ble_read_buffer_length += p_evt_write->len;

	if (p_evt_write->len == 2 && p_evt_write->data[0] == 0x03 && p_evt_write->data[1] == 0x04) {
		//got the EOS characters, write this to UART
		spi_slave_set_tx_buffer(p_client, SPI_BUS_DATA, p_client->ble_read_buffer, p_client->ble_read_buffer_length);
        p_client->ble_read_buffer_length = SPI_HEADER_SIZE;
	}
}

/**@brief Funtion for sending data to the client
 *
 * @param[in] data  Data to be sent to the client.
 * @param[in] len   Lengthof the data to be sent to the client.
 */
void client_send_data(uint8_t *data, uint16_t len)
{
    while (len > 0) {
        int chunkLength = (data[0] << 8) | data[1];
        int id = data[2];

        int err_code = 0;
        ble_gattc_write_params_t write_params;
        uint8_t buffer[20];

        int actualBytesSent = 0;

        int formattedLength = chunkLength + BLE_HEADER_SIZE;
        DEBUG("Sending data of size %d", formattedLength);
        for (int i = 0; i < formattedLength; i += 20) {
            uint16_t size = (formattedLength - i > 20 ? 20 : formattedLength - i);
            actualBytesSent += size;
            memcpy(buffer, data + i + SPI_HEADER_SIZE, size);
            DEBUG("Sending packaet of size %d", size);
            write_params.write_op = BLE_GATT_OP_WRITE_CMD;
            write_params.handle = m_client[id].srv_db.services[0].charateristics[m_client[id].up_char_index].characteristic.handle_value;
            write_params.offset = 0;
            write_params.len = size;
            write_params.p_value = buffer;

            err_code = sd_ble_gattc_write(m_client[id].srv_db.conn_handle, &write_params);
            while (err_code == BLE_ERROR_NO_TX_BUFFERS) {
                uint8_t bufferCount = 0;
                while (waitForTxComplete) {
                    sd_ble_tx_buffer_count_get(&bufferCount);
                    nrf_delay_us(500);
                }
                waitForTxComplete = true;
                err_code = sd_ble_gattc_write(m_client[id].srv_db.conn_handle, &write_params);
            }
        }

        //now send the EOS characters
        uint8_t eotBuffer[2] = {3, 4};
        write_params.write_op = BLE_GATT_OP_WRITE_CMD;
        write_params.handle = m_client[id].srv_db.services[0].charateristics[m_client[id].up_char_index].characteristic.handle_value;
        write_params.offset = 0;
        write_params.len = 2;
        write_params.p_value = eotBuffer;

        err_code = sd_ble_gattc_write(m_client[id].srv_db.conn_handle, &write_params);
        while (err_code == BLE_ERROR_NO_TX_BUFFERS) {
            while (waitForTxComplete) {
                nrf_delay_us(500);
            }
            waitForTxComplete = true;
            err_code = sd_ble_gattc_write(m_client[id].srv_db.conn_handle, &write_params);
        }
        len-=(chunkLength+SPI_HEADER_SIZE + BLE_HEADER_SIZE);
    }
}


static void db_discovery_evt_handler(ble_db_discovery_evt_t * p_evt)
{
    // Find the client using the connection handle.
    client_t * p_client;
    uint32_t   index;
    bool       is_valid_srv_found = false;

    index = client_find(p_evt->conn_handle);
    p_client = &m_client[index];
    //blink(1);

    if (p_evt->evt_type == BLE_DB_DISCOVERY_COMPLETE)
    {
        uint8_t i;
        for (i = 0; i < p_evt->params.discovered_db.char_count; i++)
        {
            ble_db_discovery_char_t * p_characteristic;

            p_characteristic = &(p_evt->params.discovered_db.charateristics[i]);

            if ((p_characteristic->characteristic.uuid.uuid == BLE_SCS_UUID_DATA_DN_CHAR)
                &&
                (p_characteristic->characteristic.uuid.type == m_base_uuid_type))
            {
                // Characteristic found. Store the information needed and break.
                p_client->dn_char_index = i;
                is_valid_srv_found   = true;
            }
            else if ((p_characteristic->characteristic.uuid.uuid == BLE_SCS_UUID_DATA_UP_CHAR)
				&&
				(p_characteristic->characteristic.uuid.type == m_base_uuid_type))
			{
				// Characteristic found. Store the information needed and break.
				p_client->up_char_index = i;
				is_valid_srv_found   = true;
			}
        }
    }

    if (is_valid_srv_found)
    {
        // Enable notification.
        notif_enable(p_client);
    }
    else
    {
        p_client->state = STATE_ERROR;
    }
}


/**@brief Function for setting client to the running state once write response is received.
 *
 * @param[in] p_ble_evt Event to handle.
 */
static void on_evt_write_rsp(ble_evt_t * p_ble_evt, client_t * p_client)
{
    if ((p_client != NULL) && (p_client->state == STATE_NOTIF_ENABLE))
    {
        if (p_ble_evt->evt.gattc_evt.params.write_rsp.handle !=
            p_client->srv_db.services[0].charateristics[p_client->dn_char_index].cccd_handle)
        {
            // Got response from unexpected handle.
            p_client->state = STATE_ERROR;
        }
        else
        {
            p_client->state = STATE_RUNNING;
            p_client->peripheralConnected = true;
        }
    }
}


/**@brief Function for toggling LEDS based on received notifications.
 *
 * @param[in] p_ble_evt Event to handle.
 */
static void on_evt_hvx(ble_evt_t * p_ble_evt, client_t * p_client, uint32_t index)
{
    if ((p_client != NULL) && (p_client->state == STATE_RUNNING))
    {
        if (p_ble_evt->evt.gattc_evt.params.hvx.handle ==
                                p_client->srv_db.services[0].charateristics[p_client->dn_char_index].characteristic.handle_value)
        {
			ble_gattc_evt_hvx_t * p_evt_write = &p_ble_evt->evt.gattc_evt.params.hvx;

			if (p_evt_write->len == 2 && p_evt_write->data[0] == 0x03 && p_evt_write->data[1] == 0x04) {
				if ( p_client->peripheralConnected && !p_client->socketedParticle) {
                    p_client->socketedParticle = true;

                    //this is a hack-fx. v1.0.47 of bluz FW didn't properly fill out the connection field of the BLE header, so we have to do it here
                    p_client->ble_read_buffer[SPI_HEADER_SIZE+1] = ((SPI_BUS_CONNECT << 4) & 0xF0) | (p_client->ble_read_buffer[1] & 0x0F);

                    spi_slave_set_tx_buffer(p_client, SPI_BUS_CONNECT, p_client->ble_read_buffer, p_client->ble_read_buffer_length);
				} else {
					//got the EOS characters, write this to UART
					spi_slave_set_tx_buffer(p_client, SPI_BUS_DATA, p_client->ble_read_buffer, p_client->ble_read_buffer_length);
				}
                p_client->ble_read_buffer_length = SPI_HEADER_SIZE;
			} else {
				memcpy(p_client->ble_read_buffer+p_client->ble_read_buffer_length, p_evt_write->data, p_evt_write->len);
                p_client->ble_read_buffer_length += p_evt_write->len;
			}
        }
    }
}


/**@brief Function for handling timeout events.
 */
static void on_evt_timeout(ble_evt_t * p_ble_evt, client_t * p_client)
{
    APP_ERROR_CHECK_BOOL(p_ble_evt->evt.gattc_evt.params.timeout.src
                         == BLE_GATT_TIMEOUT_SRC_PROTOCOL);

    if (p_client != NULL)
    {
        p_client->state = STATE_ERROR;
    }
}


ret_code_t client_handling_dm_event_handler(const dm_handle_t    * p_handle,
                                              const dm_event_t     * p_event,
                                              const ret_code_t     event_result)
{
    client_t * p_client = &m_client[p_handle->connection_id];

    switch (p_event->event_id)
    {
       case DM_EVT_LINK_SECURED:
           // Attempt configuring CCCD now that bonding is established.
           if (event_result == NRF_SUCCESS)
           {
               notif_enable(p_client);
           }
           break;
       default:
           break;
    }

    return NRF_SUCCESS;
}


void client_handling_ble_evt_handler(ble_evt_t * p_ble_evt)
{
    client_t * p_client = NULL;
    uint32_t index = client_find(p_ble_evt->evt.gattc_evt.conn_handle);
    if (index != MAX_CLIENTS)
    {
       p_client = &m_client[index];
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GATTC_EVT_WRITE_RSP:
            if ((p_ble_evt->evt.gattc_evt.gatt_status == BLE_GATT_STATUS_ATTERR_INSUF_AUTHENTICATION) ||
                (p_ble_evt->evt.gattc_evt.gatt_status == BLE_GATT_STATUS_ATTERR_INSUF_ENCRYPTION))
            {
                uint32_t err_code = dm_security_setup_req(&p_client->handle);
                APP_ERROR_CHECK(err_code);

            }
            on_evt_write_rsp(p_ble_evt, p_client);
            break;

        case BLE_GATTS_EVT_WRITE:
        	on_write(p_client, p_ble_evt);
        	break;

        case BLE_GATTC_EVT_HVX:
            on_evt_hvx(p_ble_evt, p_client, index);
            break;

        case BLE_GATTC_EVT_TIMEOUT:
            on_evt_timeout(p_ble_evt, p_client);
            break;

        case BLE_EVT_TX_COMPLETE:
			waitForTxComplete = false;
			break;

        case BLE_GAP_EVT_DISCONNECTED:
			break;

        case BLE_GAP_EVT_CONNECTED:
			break;

        default:
            break;
    }


    if (p_client != NULL)
    {
        ble_db_discovery_on_ble_evt(&(p_client->srv_db), p_ble_evt);
    }
}


/**@brief Database discovery module initialization.
 */
static void db_discovery_init(void)
{
    uint32_t err_code = ble_db_discovery_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the client handling.
 */
void client_handling_init(void (*b)(uint8_t *m_tx_buf, uint16_t size))
{
    blink_led(1);
	tx_callback = b;

	uint32_t err_code;
    uint32_t i;

    ble_uuid128_t base_uuid = MULTILINK_PERIPHERAL_BASE_UUID;

    err_code = sd_ble_uuid_vs_add(&base_uuid, &m_base_uuid_type);
    APP_ERROR_CHECK(err_code);

    for (i = 0; i < MAX_CLIENTS; i++)
    {
        m_client[i].state  = IDLE;
    }

    m_client_count = 0;

    db_discovery_init();

    // Register with discovery module for the discovery of the service.
    ble_uuid_t uuid;

    uuid.type = m_base_uuid_type;
    uuid.uuid = BLE_SCS_UUID_SERVICE;

    err_code = ble_db_discovery_evt_register(&uuid,
                                             db_discovery_evt_handler);

    APP_ERROR_CHECK(err_code);
}

/**@brief Function for returning the current number of clients.
 */
uint8_t client_handling_count(void)
{
    return m_client_count;
}


/**@brief Function for creating a new client.
 */
uint32_t client_handling_create(const dm_handle_t * p_handle, uint16_t conn_handle)
{
    m_client[p_handle->connection_id].ble_read_buffer_length = SPI_HEADER_SIZE;
    m_client[p_handle->connection_id].state              = STATE_SERVICE_DISC;
    m_client[p_handle->connection_id].srv_db.conn_handle = conn_handle;
                m_client_count++;
    m_client[p_handle->connection_id].handle             = (*p_handle);
    m_client[p_handle->connection_id].id                 = p_handle->connection_id;
    m_client[p_handle->connection_id].socketedParticle   = false;
    m_client[p_handle->connection_id].peripheralConnected = false;
    service_discover(&m_client[p_handle->connection_id]);

    return NRF_SUCCESS;
}


/**@brief Function for freeing up a client by setting its state to idle.
 */
uint32_t client_handling_destroy(const dm_handle_t * p_handle)
{
    uint32_t      err_code = NRF_SUCCESS;
    client_t    * p_client = &m_client[p_handle->connection_id];

    if (p_client->state != IDLE)
    {
        m_client_count--;
        p_client->state = IDLE;
        //first 3 bytes are SPI header, will get filled in by function. next two bytes are BLE header
        uint8_t dummy[6] = {0, 0, 0, SOCKET_DATA_SERVICE, (((SPI_BUS_DISCONNECT << 4) & 0xF0) | (0 & 0x0F)), 22};
        spi_slave_set_tx_buffer(p_client, SPI_BUS_DISCONNECT, dummy, 6);
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }
    return err_code;
}

void disconnect_all_peripherals(void) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (m_client[i].state == STATE_RUNNING) {
            sd_ble_gap_disconnect(m_client[i].srv_db.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        }

    }
}
