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

#ifndef __BLE_STREAM_H
#define __BLE_STREAM_H

#include "ble_nus.h"

#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */
#define BLE_STREAM_BUFFER_SIZE          48

ble_nus_t m_nus;
uint8_t ble_stream_buffer[BLE_STREAM_BUFFER_SIZE];
uint16_t ble_stream_buffer_start, ble_stream_buffer_length;


void nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);

void ble_stream_begin(void);
uint16_t ble_stream_write(uint8_t * p_string, uint16_t length);
uint8_t ble_stream_read(void);
uint8_t ble_stream_peek(void);
uint8_t ble_stream_available(void);
uint8_t ble_stream_available_to_write(void);

#endif /*__BLE_STREAM_H*/