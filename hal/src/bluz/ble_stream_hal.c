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

#include "ble_stream.h"

void HAL_BLE_STREAM_Init() { }
void HAL_BLE_STREAM_Begin() { ble_stream_begin(); }
void HAL_BLE_STREAM_End() { }

uint32_t HAL_BLE_STREAM_Write_Data(uint8_t data)
{
    uint8_t bytes[1] = {data};
    return ble_stream_write(bytes, sizeof(bytes));
}

int32_t HAL_BLE_STREAM_Available_Data()
{
    return ble_stream_available();
}

int32_t HAL_BLE_STREAM_Available_Data_For_Write()
{
    return ble_stream_available_to_write();
}

int32_t HAL_BLE_STREAM_Read_Data()
{
    return ble_stream_read();
}

int32_t HAL_BLE_STREAM_Peek_Data()
{
    return ble_stream_peek();
}

void HAL_BLE_STREAM_Flush_Data() { }
bool HAL_BLE_STREAM_Is_Enabled()
{
    return true;
}
