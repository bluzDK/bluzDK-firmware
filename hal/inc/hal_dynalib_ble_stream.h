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

#ifndef HAL_DYNALIB_BLE_STREAM_H
#define	HAL_DYNALIB_BLE_STREAM_H

#include "dynalib.h"

#ifdef DYNALIB_EXPORT
#include "ble_stream_hal.h"
#endif

// WARNING
// The order of functions must not be changed or older applications will break
// when used with newer system firmware.
// Function signatures shouldn't be changed other than changing pointer types.
// New HAL functions must be added to the end of this list.
// GNINRAW

DYNALIB_BEGIN(hal_ble_stream)

DYNALIB_FN(0, hal_ble_stream,HAL_BLE_STREAM_Init, void(void))
DYNALIB_FN(1, hal_ble_stream,HAL_BLE_STREAM_Begin, void(void))
DYNALIB_FN(2, hal_ble_stream,HAL_BLE_STREAM_End, void(void))
DYNALIB_FN(3, hal_ble_stream,HAL_BLE_STREAM_Write_Data, uint32_t(uint8_t))
DYNALIB_FN(4, hal_ble_stream,HAL_BLE_STREAM_Available_Data, int32_t(void))
DYNALIB_FN(5, hal_ble_stream,HAL_BLE_STREAM_Available_Data_For_Write, int32_t(void))
DYNALIB_FN(6, hal_ble_stream,HAL_BLE_STREAM_Read_Data, int32_t(void))
DYNALIB_FN(7, hal_ble_stream,HAL_BLE_STREAM_Peek_Data, int32_t(void))
DYNALIB_FN(8, hal_ble_stream,HAL_BLE_STREAM_Flush_Data, void(void))
DYNALIB_FN(9, hal_ble_stream,HAL_BLE_STREAM_Is_Enabled, bool(void))

DYNALIB_END(hal_ble_stream)

#endif	/* HAL_DYNALIB_BLE_STREAM_H */

