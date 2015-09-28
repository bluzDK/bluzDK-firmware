/**
 ******************************************************************************
 * @file    deviceid_hal.cpp
 * @author  Matthew McGowan
 * @version V1.0.0
 * @date    27-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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

#include "deviceid_hal.h"
#include <stddef.h>
#include "nrf51.h"

#undef STATIC_ASSERT
#include "hw_config.h"

unsigned HAL_device_ID(uint8_t* dest, unsigned destLen)
{
    if (dest==NULL && destLen==0) {
        return 12;
    }
    
    dest[0] = 0xb1;
    dest[1] = 0xe2;
    
    //now, we have a 16-bit integer stored in 0x1000, so let's read that
    uint16_t deviceInt = FLASH_GetDeviceInt();
    dest[2] = (deviceInt & 0xFF);
    dest[3] = (deviceInt & 0xFF00) >> 8;
    
    //finally, add the nrf51 device ID's as the last 8 bytes, in order: DEVICEID[1] DEVICEID[0]
    dest[4] = (NRF_FICR->DEVICEID[1] & 0xFF);
    dest[5] = (NRF_FICR->DEVICEID[1] & 0xFF00) >> 8;
    dest[6] = (NRF_FICR->DEVICEID[1] & 0xFF0000) >> 16;
    dest[7] = (NRF_FICR->DEVICEID[1] & 0xFF000000) >> 24;
    
    dest[8] = (NRF_FICR->DEVICEID[0] & 0xFF);
    dest[9] = (NRF_FICR->DEVICEID[0] & 0xFF00) >> 8;
    dest[10] = (NRF_FICR->DEVICEID[0] & 0xFF0000) >> 16;
    dest[11] = (NRF_FICR->DEVICEID[0] & 0xFF000000) >> 24;
        
    return 12;
}

unsigned HAL_Platform_ID()
{
    return PLATFORM_ID;
}
