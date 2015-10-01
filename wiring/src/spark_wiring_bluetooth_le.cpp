/**
 ******************************************************************************
 * @file    spark_wiring_bluetooth_le.cpp
 * @author  Eric Ely
 * @version V1.0.0
 * @date    1-October-2015
 * @brief   Wrapper for wiring BLE module
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

#include "spark_wiring_bluetooth_le.h"
#include "core_hal.h"

void BLEClass::startAdvertising()
{
    HAL_BLE_Start_Advertising();
}

void BLEClass::stopAdvertising()
{
    HAL_BLE_Stop_Advertising();
}

bool BLEClass::isAdvertising()
{
    return HAL_BLE_Is_Advertising();
}