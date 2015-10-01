/**
 ******************************************************************************
 * @file    bluetooth_ls_hal.c
 * @author  Eric Ely
 * @version V1.0.0
 * @date    1-October-2015
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 Easier to Use, LLC  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "spi_master.h"

void HAL_BLE_Start_Advertising(void)
{
    advertising_start();
}
void HAL_BLE_Stop_Advertising(void)
{
    advertising_stop();
}
bool HAL_BLE_Is_Advertising(void)
{
    return state == BLE_ADVERTISING;
}