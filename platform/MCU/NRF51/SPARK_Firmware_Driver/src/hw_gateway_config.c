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

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "pstorage.h"
#include "hw_gateway_config.h"
#include "client_handling.h"
#include "spi_slave_stream.h"
#include "nrf51_config.h"
#include "nrf51_callbacks.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "data_management_layer.h"
#include "registered_data_services.h"

#include "debug.h"

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
void ble_gateway_stack_init(void)
{
    uint32_t err_code;

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, false);

    // Enable BLE stack.
    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));

    ble_enable_params.gatts_enable_params.service_changed = false;
    ble_enable_params.gap_enable_params.role              = BLE_GAP_ROLE_CENTRAL;

    err_code = sd_ble_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for System events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}

void gateway_notify_disconnect_all(void) {
    for (int i = 0; i <= MAX_CLIENTS; i++) {
        uint8_t dummy[6] = {(( (6-SPI_HEADER_SIZE-BLE_HEADER_SIZE) & 0xFF00) >> 8), ( (6-SPI_HEADER_SIZE-BLE_HEADER_SIZE) & 0xFF), i, SOCKET_DATA_SERVICE, (((SPI_BUS_DISCONNECT << 4) & 0xF0) | (0 & 0x0F)), 22};
        spi_slave_send_data(dummy, 6);
    }
}

//HW Init Functions
void gateway_init(void)
{
    memset(&m_connection_param, 0, sizeof(m_connection_param));
    m_connection_param.min_conn_interval = (uint16_t)MIN_CONNECTION_INTERVAL;
    m_connection_param.max_conn_interval = (uint16_t)MAX_CONNECTION_INTERVAL;
    m_connection_param.slave_latency = 0;
    m_connection_param.conn_sup_timeout = (uint16_t)SUPERVISION_TIMEOUT;

    lastConnectionErrorTime = 0;
    connectionErrors = 0;
    lastConnectionTime = 0;
    m_peer_count = 0;
    m_memory_access_in_progress = false;
    spi_slave_tx_buffer_size = 0;
    spi_slave_tx_buffer_start = 0;
    
    spi_slave_rx_buffer_size = 0;
    spi_slave_rx_buffer_start = 0;

    info_data_service_buffer_size = 0;
    
    client_handling_init(spi_slave_tx_data);
    spi_slave_stream_init(spi_slave_rx_data);
    gateway_notify_disconnect_all();

}

//starts scanning for peripherals
void gateway_scan_start(void)
{
    uint32_t err_code;
    uint32_t count;
    // Verify if there is any flash access pending, if yes delay starting scanning until
    // it's complete.
    err_code = pstorage_access_status_get(&count);
    APP_ERROR_CHECK(err_code);
    
    if (count != 0)
    {
        m_memory_access_in_progress = true;
        return;
    }
    err_code = sd_ble_gap_scan_start(&m_scan_param);
    APP_ERROR_CHECK(err_code);
    state = BLE_SCANNING;
}

//interrupt driven function to put data into buffers for process in gateway_loop
void spi_slave_tx_data(uint8_t* tx_buffer, uint16_t size)
{
    if (spi_slave_tx_buffer_start+spi_slave_tx_buffer_size+size <= SPI_SLAVE_TX_BUF_SIZE) {
        memcpy(spi_slave_tx_buffer+spi_slave_tx_buffer_start+spi_slave_tx_buffer_size, tx_buffer, size);
        spi_slave_tx_buffer_size += size;
    }
}

void spi_slave_rx_data(uint8_t *rx_buffer, uint16_t size)
{
    //this data is for the gateways cloud connection
    //hack - set the third byte to the socket ID, which is always 0 for now
    //then send only the data starting at the third byte, which is 1 byte longer than the length reported
    if (rx_buffer[2] == GATEWAY_ID) {
        int serviceID = rx_buffer[3];
        int length = (rx_buffer[0] << 8) | rx_buffer[1];
        if (serviceID == INFO_DATA_SERVICE) {
            info_data_service_buffer_size = length;
            memcpy(info_data_service_buffer, rx_buffer, size);
        } else {
            dataManagementFeedData(length + BLE_HEADER_SIZE, rx_buffer + SPI_HEADER_SIZE);
        }
    } else {
        if (spi_slave_rx_buffer_start+spi_slave_rx_buffer_size+size <= SPI_SLAVE_RX_BUF_SIZE) {
            memcpy(spi_slave_rx_buffer+spi_slave_rx_buffer_start+spi_slave_rx_buffer_size, rx_buffer, size);
            spi_slave_rx_buffer_size += size;
        }
    }
}

//needs to be called in the main loop to process data through the gateway
void gateway_loop(void)
{
    if (spi_slave_rx_buffer_size > 0) {
        int id = spi_slave_rx_buffer[2];
        int bytesRx = spi_slave_rx_buffer_size;
        DEBUG("Starting!");
        if (id < MAX_CLIENTS) {
            DEBUG("Sending down data of size %d on id %d", spi_slave_rx_buffer_size, id);
            //this data is for one of the connected bluz DK boards
            client_send_data(spi_slave_rx_buffer + spi_slave_rx_buffer_start, bytesRx);
            spi_slave_rx_buffer_size -= bytesRx;
            spi_slave_rx_buffer_start += bytesRx;
            if (spi_slave_rx_buffer_size == 0) {
                spi_slave_rx_buffer_start = 0;
            }
        }
    }
    
    if (spi_slave_tx_buffer_size > 0) {
        int bytesTx = spi_slave_tx_buffer_size;
        spi_slave_send_data(spi_slave_tx_buffer+spi_slave_tx_buffer_start, bytesTx);

        spi_slave_tx_buffer_size -= bytesTx;
        spi_slave_tx_buffer_start += bytesTx;
        if (spi_slave_tx_buffer_size == 0) {
            spi_slave_tx_buffer_start = 0;
        }
    }

    if (info_data_service_buffer_size > 0) {
        int length = (info_data_service_buffer[0] << 8) | info_data_service_buffer[1];
        dataManagementFeedData(length + BLE_HEADER_SIZE, info_data_service_buffer + SPI_HEADER_SIZE);
        info_data_service_buffer_size = 0;
    }

    //sort of a connection watchdog timer
    //if a connection timed out, but didn't explicitly fail, then scanning would never start again
    if (state==BLE_OFF && m_peer_count < MAX_CLIENTS) {
        if (lastConnectionErrorTime == 0) {
            lastConnectionErrorTime = system_millis();
        }
        if (system_millis() - lastConnectionErrorTime > (connectionErrors*1000)) {
            connectionErrors++;
        }
        if (connectionErrors > CONNECTION_FAILURE_TIMEOUT) {
            gateway_cancel_connect_and_start_scanning();
        }
    } else if (state==BLE_SCANNING) {
        if (connectionErrors != 0) {
            connectionErrors = 0;
            lastConnectionErrorTime = 0;
        }
    }
}

void gateway_cancel_connect_and_start_scanning(void)
{
    sd_ble_gap_connect_cancel();
    gateway_scan_start();
    connectionErrors = 0;
    lastConnectionErrorTime = 0;
}

//parse the advertisement data for type
uint32_t adv_report_parse(uint8_t type, data_t * p_advdata, data_t * p_typedata)
{
    uint32_t index = 0;
    uint8_t * p_data;
    
    p_data = p_advdata->p_data;
    
    while (index < p_advdata->data_len)
    {
        uint8_t field_length = p_data[index];
        uint8_t field_type   = p_data[index+1];
        
        if (field_type == type)
        {
            p_typedata->p_data   = &p_data[index+2];
            p_typedata->data_len = field_length-1;
            return NRF_SUCCESS;
        }
        index += field_length+1;
    }
    return NRF_ERROR_NOT_FOUND;
}

void setGatewayConnParameters(int minimum, int maximum)
{
    disconnect_all_peripherals();
    m_connection_param.min_conn_interval = MSEC_TO_UNITS(minimum, UNIT_1_25_MS);
    m_connection_param.max_conn_interval = MSEC_TO_UNITS(maximum, UNIT_1_25_MS);
}