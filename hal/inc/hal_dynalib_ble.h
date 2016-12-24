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

DYNALIB_FN(0, hal_ble,HAL_BLE_GET_STATE, BLUETOOTH_LE_STATE(void))
DYNALIB_FN(1, hal_ble,HAL_BLE_GET_CONNECTION_INTERVAL, uint32_t(void))
DYNALIB_FN(2, hal_ble,HAL_BLE_Start_Advertising, void(void))
DYNALIB_FN(3, hal_ble,HAL_BLE_Stop_Advertising, void(void))
DYNALIB_FN(4, hal_ble,HAL_BLE_Disconnect, void(void))
DYNALIB_FN(5, hal_ble,HAL_BLE_Register_Radio_Notification, int(void (*radio_callback)(bool radio_active)))
DYNALIB_FN(6, hal_ble,HAL_BLE_Register_Data_Callback, void(void (*data_callback)(uint8_t *data, uint16_t length)))
DYNALIB_FN(7, hal_ble,HAL_BLE_Send_Data, void(uint8_t *data, uint16_t length))
DYNALIB_FN(8, hal_ble,HAL_BLE_Set_TX_Power, void(int power))
DYNALIB_FN(9, hal_ble,HAL_BLE_Set_CONN_PARAMS, void(int minimum, int maximum))
DYNALIB_FN(10, hal_ble,HAL_BLE_Set_Adv_Name, void(char* name))
DYNALIB_FN(11, hal_ble,HAL_BLE_Set_Gateway_Target, void(char* name))
DYNALIB_FN(12, hal_ble,HAL_BLE_Start_iBeacon, void(uint32_t major, uint32_t minor, uint8_t *UUID))
DYNALIB_END(hal_ble)

#endif	/* HAL_DYNALIB_BLE_H */

