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

/* Define to prevent recursive inclusion -------------------------------------*/
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "flash.h"
#include "module_info.h"
#include "sst25vf_spi.h"
#undef STATIC_ASSERT
#include "hw_config.h"

bool FLASH_isUserModuleInfoValid(uint8_t flashDeviceID, uint32_t startAddress, uint32_t expectedAddress)
{
    const module_info_t* module_info = FLASH_ModuleInfo(flashDeviceID, startAddress);
    
    return (module_info != NULL
            && ((uint32_t)(module_info->module_end_address)<=0x40000)
            && ((uint32_t)(module_info->module_start_address)>=0x18000)
            && (module_info->platform_id==PLATFORM_ID));
}

const module_info_t* FLASH_ModuleInfo(uint8_t flashDeviceID, uint32_t startAddress)
{
    if (flashDeviceID == FLASH_INTERNAL) {
        //read the buffer from internal flash
        if (startAddress >= 0x40000) {
            return NULL;
        }
//        memcpy(buf, (const void *)startAddress, 256);
        if (((*(volatile uint32_t*)startAddress) & APP_START_MASK) == 0x20000000)
        {
            startAddress += 0xc0;
        }
        
        const module_info_t* module_info = (const module_info_t*)startAddress;
        return module_info;
    } else {
        //largest possible location for module_info is 0xc0 + 0x18, so 256 bytes is plenty
        uint8_t buf[256];
        uint32_t moduleInfoAddress = 0;
        
        //read the buffer from external flash
        sFLASH_ReadBuffer(buf, startAddress, 256);
        
        uint32_t firstInt = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8)  |  buf[0];
        
        //if it is the vector table, then look past it for module_info
        if ((firstInt & APP_START_MASK) == 0x20000000)
        {
            moduleInfoAddress += 0xc0;
        }
        
        const module_info_t *module_info = (const module_info_t*)(buf+moduleInfoAddress);
        return module_info;
        
    }
}

inline uint32_t decode_uint32(unsigned char* buf) {
    return buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3];
}

bool FLASH_VerifyCRC32(uint8_t flashDeviceID, uint32_t startAddress, uint32_t length)
{
    if(flashDeviceID == FLASH_INTERNAL && length > 0)
    {
//        uint32_t expectedCRC = __REV((*(__IO uint32_t*) (startAddress + length)));
        uint32_t expectedCRC = decode_uint32((uint8_t*)(startAddress+length));
        uint32_t computedCRC = Compute_CRC32(0, (uint8_t*)startAddress, length);
        
        if (expectedCRC == computedCRC)
        {
            return true;
        }
    }
    
    return false;
}

uint32_t FLASH_ModuleLength(uint8_t flashDeviceID, uint32_t startAddress)
{
    const module_info_t* module_info = FLASH_ModuleInfo(flashDeviceID, startAddress);
    
    if (module_info != NULL)
    {
        return ((uint32_t)module_info->module_end_address - (uint32_t)module_info->module_start_address);
    }
    
    return 0;
}
