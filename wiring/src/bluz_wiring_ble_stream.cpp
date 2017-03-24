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

#include "bluz_wiring_ble_stream.h"

BLEStream::BLEStream()
{
    HAL_BLE_STREAM_Init();
}

void BLEStream::begin()
{
    HAL_BLE_STREAM_Begin();
}

void BLEStream::end()
{
    HAL_BLE_STREAM_End();
}

int BLEStream::available(void)
{
    return HAL_BLE_STREAM_Available_Data();
}

size_t BLEStream::write(uint8_t c)
{
    // attempt a write if blocking, or for non-blocking if there is room.
    if (HAL_BLE_STREAM_Available_Data_For_Write() > 0) {
        // the HAL always blocks.
        return HAL_BLE_STREAM_Write_Data(c);
    }
    return 0;
}

int BLEStream::peek(void)
{
    return HAL_BLE_STREAM_Peek_Data();
}

int BLEStream::read(void)
{
    return HAL_BLE_STREAM_Read_Data();
}

void BLEStream::flush()
{
    HAL_BLE_STREAM_Flush_Data();
}

bool BLEStream::isEnabled() {
    return HAL_BLE_STREAM_Is_Enabled();
}

