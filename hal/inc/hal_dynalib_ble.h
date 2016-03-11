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

#ifndef HAL_DYNALIB_BLE_H
#define	HAL_DYNALIB_BLE_H

#include "dynalib.h"

#ifdef DYNALIB_EXPORT
#include "bluetooth_le_hal.h"
#endif

// WARNING
// The order of functions must not be changed or older applications will break
// when used with newer system firmware.
// Function signatures shouldn't be changed other than changing pointer types.
// New HAL functions must be added to the end of this list.
// GNINRAW

DYNALIB_BEGIN(hal_ble)
DYNALIB_FN(hal_ble,HAL_BLE_GET_STATE)
DYNALIB_FN(hal_ble,HAL_BLE_GET_CONNECTION_INTERVAL)
DYNALIB_FN(hal_ble,HAL_BLE_Start_Advertising)
DYNALIB_FN(hal_ble,HAL_BLE_Stop_Advertising)
DYNALIB_FN(hal_ble,HAL_BLE_Disconnect)
DYNALIB_FN(hal_ble,HAL_BLE_Register_Radio_Notification)
DYNALIB_FN(hal_ble,HAL_BLE_Register_Data_Callback)
DYNALIB_FN(hal_ble,HAL_BLE_Send_Data)
DYNALIB_END(hal_ble)

#endif	/* HAL_DYNALIB_BLE_H */

