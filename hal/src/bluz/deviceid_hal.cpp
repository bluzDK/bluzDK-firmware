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
#include "debug.h"
#include <stddef.h>
#include <cstring>

extern "C" {
#include "nrf51.h"
}

unsigned HAL_device_ID(uint8_t* dest, unsigned destLen)
{
    if (dest==NULL && destLen==0) {
        return 12;
    }
    
    DEBUG("about to try...");
    dest[0] = 0xb1;
    dest[1] = 0xe2;
    
    //now, we have a 16-bit integer stored in 0x3F000, so let's read that
    char buf[2];
    DEBUG("memcpy...");
    memcpy(buf, (const void *)0x3F000, 2);
    DEBUG("memcpy done...");
    dest[2] = buf[1];
    dest[3] = buf[0];
    
    DEBUG("copying registers...");
    //finally, add the nrf51 device ID's as the last 8 bytes, in order: DEVICEID[1] DEVICEID[0]
    dest[4] = (NRF_FICR->DEVICEID[1] & 0xFF);
    dest[5] = (NRF_FICR->DEVICEID[1] & 0xFF00) >> 8;
    dest[6] = (NRF_FICR->DEVICEID[1] & 0xFF0000) >> 16;
    dest[7] = (NRF_FICR->DEVICEID[1] & 0xFF000000) >> 24;
    
    dest[8] = (NRF_FICR->DEVICEID[0] & 0xFF);
    dest[9] = (NRF_FICR->DEVICEID[0] & 0xFF00) >> 8;
    dest[10] = (NRF_FICR->DEVICEID[0] & 0xFF0000) >> 16;
    dest[11] = (NRF_FICR->DEVICEID[0] & 0xFF000000) >> 24;
    DEBUG("copying registers done...");
            
    return 12;
}

unsigned HAL_Platform_ID()
{
    return PLATFORM_ID;
}
