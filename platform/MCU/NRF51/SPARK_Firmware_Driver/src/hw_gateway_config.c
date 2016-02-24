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
#include "hw_gateway_config.h"
#include "client_handling.h"
#include "spi_slave_stream.h"

//HW Init Functions
void gateway_init(void)
{
    spi_slave_tx_buffer_size = 0;
    spi_slave_tx_buffer_start = 0;
    
    spi_slave_rx_buffer_size = 0;
    spi_slave_rx_buffer_start = 0;
    
    client_handling_init(spi_slave_tx_data);
    spi_slave_stream_init(spi_slave_rx_data);
}


void spi_slave_tx_data(uint8_t *tx_buffer, uint16_t size)
{
    memcpy(spi_slave_tx_buffer+spi_slave_tx_buffer_start+spi_slave_tx_buffer_size, tx_buffer, size);
    spi_slave_tx_buffer_size += size;
}

void spi_slave_rx_data(uint8_t *rx_buffer, uint16_t size)
{
    memcpy(spi_slave_rx_buffer+spi_slave_rx_buffer_start+spi_slave_rx_buffer_size, rx_buffer, size);
    spi_slave_rx_buffer_size += size;
}