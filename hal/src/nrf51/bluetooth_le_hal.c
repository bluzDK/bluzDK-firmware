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
#include "bluetooth_le_hal.h"
#include "hw_config.h"
#include "hw_gateway_config.h"
#include "spi_master.h"
#include "nrf51_config.h"

BLUETOOTH_LE_STATE HAL_BLE_GET_STATE(void)
{
    return state;
}

uint32_t HAL_BLE_GET_CONNECTION_INTERVAL(void)
{
    return system_connection_interval;
}

void HAL_BLE_Start_Advertising(void)
{
    advertising_start();
}
void HAL_BLE_Stop_Advertising(void)
{
    advertising_stop();
}

void HAL_BLE_Disconnect(void)
{
    ble_disconnect();
}

int HAL_BLE_Register_Radio_Notification(void (*radio_callback)(bool radio_active))
{
    return register_radio_callback(radio_callback);
}

void HAL_BLE_Register_Data_Callback(void (*data_callback)(uint8_t *data, uint16_t length))
{
    register_data_callback(data_callback);
}

void HAL_BLE_Send_Data(uint8_t *data, uint16_t length)
{
    send_data(data, length);
}

void HAL_BLE_Set_TX_Power(int power)
{
    setTxPower(power);
}

void HAL_BLE_Set_CONN_PARAMS(int minimum, int maximum)
{
#if PLATFORM_ID==103
    setConnParameters(minimum, maximum);
#endif
#if PLATFORM_ID==269
    setGatewayConnParameters(minimum, maximum);
#endif
}

void HAL_BLE_Set_Adv_Name(char* name)
{
    setAdvertisedName(name);
}