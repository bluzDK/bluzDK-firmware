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

//Gateway Constants
#define SPI_SLAVE_TX_BUF_SIZE   1024u                       /**< SPI TX buffer size. */
#define SPI_SLAVE_RX_BUF_SIZE   SPI_SLAVE_TX_BUF_SIZE       /**< SPI RX buffer size. */

//Gateway Init Functions
void gateway_init(void);

//Gateway Callback Functions
#if PLATFORM_ID==269
uint8_t spi_slave_tx_buffer[SPI_SLAVE_TX_BUF_SIZE];
volatile uint16_t spi_slave_tx_buffer_size;
volatile uint16_t spi_slave_tx_buffer_start;
void spi_slave_tx_data(uint8_t *tx_buffer, uint16_t size);

uint8_t spi_slave_rx_buffer[SPI_SLAVE_RX_BUF_SIZE];
volatile uint16_t spi_slave_rx_buffer_size;
volatile uint16_t spi_slave_rx_buffer_start;
void spi_slave_rx_data(uint8_t *rx_buffer, uint16_t size);
#endif


#endif  /*__NRF_HW_GATEWAY_CONFIG_H*/
