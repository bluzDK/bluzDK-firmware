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

#include <string.h>
#include "ble_stream.h"

void ble_stream_begin(void)
{
    ble_stream_buffer_length = 0;
    ble_stream_buffer_start = 0;
}

void nus_data_handler(ble_nus_t * p_nus, uint8_t * data, uint16_t length)
{
    if (ble_stream_buffer_length+length > BLE_STREAM_BUFFER_SIZE) {
        return;
    }

    memcpy(ble_stream_buffer+ble_stream_buffer_start+ble_stream_buffer_length, data, length);
    ble_stream_buffer_length+=length;
}

uint16_t ble_stream_write(uint8_t * p_string, uint16_t length)
{
    return ble_nus_string_send(&m_nus, p_string, length);
}


uint8_t ble_stream_read(void)
{
    if (ble_stream_buffer_length == 0) {
        return 0;
    }

    uint8_t byte = ble_stream_buffer[ble_stream_buffer_start];
    ble_stream_buffer_start++;
    ble_stream_buffer_length--;

    if (ble_stream_buffer_length == 0) {
        ble_stream_buffer_start = 0;
    }

    return byte;
}

uint8_t ble_stream_peek(void)
{
    if (ble_stream_buffer_length == 0) {
        return 0;
    } else {
        return ble_stream_buffer[ble_stream_buffer_start];
    }
}

uint8_t ble_stream_available(void)
{
    return ble_stream_buffer_length;
}

uint8_t ble_stream_available_to_write(void)
{
    return 20;
}