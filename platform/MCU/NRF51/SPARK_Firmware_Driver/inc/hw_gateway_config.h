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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NRF_HW_GATEWAY_CONFIG_H
#define __NRF_HW_GATEWAY_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ble_gap.h"
#include "app_util.h"

#define MAX_CLIENTS  3  /**< Max number of clients. */
#define GATEWAY_ID MAX_CLIENTS

//Gateway Constants
#define SPI_SLAVE_TX_BUF_SIZE   1024                        /**< SPI TX buffer size. */
#define SPI_SLAVE_RX_BUF_SIZE   SPI_SLAVE_TX_BUF_SIZE       /**< SPI RX buffer size. */

#define MIN_CONNECTION_INTERVAL          MSEC_TO_UNITS(7.5, UNIT_1_25_MS)                /**< Determines minimum connection interval in millisecond. */
#define MAX_CONNECTION_INTERVAL          MSEC_TO_UNITS(30, UNIT_1_25_MS)                /**< Determines maximum connection interval in millisecond. */
#define SLAVE_LATENCY                    0                                              /**< Determines slave latency in counts of connection events. */
#define SUPERVISION_TIMEOUT              MSEC_TO_UNITS(4000, UNIT_10_MS)                /**< Determines supervision time-out in units of 10 millisecond. */

#define TARGET_DEV_NAME                  "Bluz DK"                                      /**< Target device name that application is looking for. */

#define SCAN_INTERVAL                    0x00A0                                         /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                      0x0050                                         /**< Determines scan window in units of 0.625 millisecond. */

uint8_t                           m_peer_count;                    /**< Number of peer's connected. */
bool                              m_memory_access_in_progress;     /**< Flag to keep track of ongoing operations on persistent memory. */

#define GATEWAY_NOTIFICATION_LED         0
#define CONNECTION_PIN					 13

#define BLE_HEADER_SIZE  2
#define SPI_HEADER_SIZE  4

/**@brief Gateway Protocol states. */
typedef enum
{
    SPI_BUS_CONNECT,
    SPI_BUS_DISCONNECT,
    SPI_BUS_DATA
} gateway_function_t;

/**@brief Variable length data encapsulation in terms of length and pointer to data */
typedef struct
{
    uint8_t     * p_data;                                                      /**< Pointer to data. */
    uint16_t      data_len;                                                    /**< Length of data. */
}data_t;

/**
 * @brief Scan parameters requested for scanning and connection.
 */
static const ble_gap_scan_params_t m_scan_param =
{
    0,                       // Active scanning not set.
    0,                       // Selective scanning not set.
    NULL,                    // White-list not set.
    (uint16_t)SCAN_INTERVAL, // Scan interval.
    (uint16_t)SCAN_WINDOW,   // Scan window.
    0                        // Never stop scanning unless explicitly asked to.
};

/**
 * @brief Connection parameters requested for connection.
 */
static const ble_gap_conn_params_t m_connection_param =
{
    (uint16_t)MIN_CONNECTION_INTERVAL,   // Minimum connection
    (uint16_t)MAX_CONNECTION_INTERVAL,   // Maximum connection
    0,                                   // Slave latency
    (uint16_t)SUPERVISION_TIMEOUT        // Supervision time-out
};

//Gateway Init Functions
void ble_gateway_stack_init(void);
void gateway_init(void);
void gateway_scan_start(void);
void gateway_loop(void);

//Gateway Callback Functions
#if PLATFORM_ID==269
uint8_t spi_slave_tx_buffer[SPI_SLAVE_TX_BUF_SIZE];
volatile uint16_t spi_slave_tx_buffer_size;
volatile uint16_t spi_slave_tx_buffer_start;
void spi_slave_tx_data(uint8_t* tx_buffer, uint16_t size);

uint8_t spi_slave_rx_buffer[SPI_SLAVE_RX_BUF_SIZE];
volatile uint16_t spi_slave_rx_buffer_size;
volatile uint16_t spi_slave_rx_buffer_start;
void spi_slave_rx_data(uint8_t *rx_buffer, uint16_t size);
#endif

uint32_t adv_report_parse(uint8_t type, data_t * p_advdata, data_t * p_typedata);

#endif  /*__NRF_HW_GATEWAY_CONFIG_H*/
