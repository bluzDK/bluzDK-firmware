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

//HW Init Functions
void gateway_init(void)
{
    m_peer_count = 0;
    m_memory_access_in_progress = false;
    spi_slave_tx_buffer_size = 0;
    spi_slave_tx_buffer_start = 0;
    
    spi_slave_rx_buffer_size = 0;
    spi_slave_rx_buffer_start = 0;
    
    client_handling_init(spi_slave_tx_data);
    spi_slave_stream_init(spi_slave_rx_data);
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
}

//interrupt driven function to put data into buffers for process in gateway_loop
void spi_slave_tx_data(uint8_t* tx_buffer, uint16_t size)
{
    DEBUG("Data->BLE Callback: %d @ %d", size, spi_slave_tx_buffer_size);
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
        int length = (rx_buffer[0] << 8) | rx_buffer[1];
        rx_buffer[3] = 0;
        dataManagementFeedData(SOCKET_DATA_SERVICE, length+1, rx_buffer+3);
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

        if (id < MAX_CLIENTS) {
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

        DEBUG("Data->SPI Function: %d  @ %d", spi_slave_tx_buffer_size, spi_slave_tx_buffer_start);
        int bytesTx = spi_slave_tx_buffer_size;
        spi_slave_send_data(spi_slave_tx_buffer+spi_slave_tx_buffer_start, bytesTx);

        spi_slave_tx_buffer_size -= bytesTx;
        spi_slave_tx_buffer_start += bytesTx;
        if (spi_slave_tx_buffer_size == 0) {
            spi_slave_tx_buffer_start = 0;
        }
    }
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
