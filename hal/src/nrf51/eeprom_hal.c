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
#include "eeprom_hal.h"
#include "hw_layout.h"
#include "sst25vf_spi.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

bool checkAddress(uint32_t address) {
    if (address >= 0 && address < HAL_EEPROM_Length()) {
        return true;
    }
    else {
        return false;
    }
}

void HAL_EEPROM_Init(void)
{
}

size_t HAL_EEPROM_Length()
{
    return USER_STORAGE_AVAILABLE;
}

uint8_t HAL_EEPROM_Read(uint32_t address)
{
    if (checkAddress(address)) {
        if (sFLASH_ReadSingleByte(FLASH_STORAGE_ADDRESS + USER_STORAGE_AVAILABLE) != 1) {
            //in case of a power failure during a write, the data may be in swap
            return sFLASH_ReadSingleByte(FLASH_STORAGE_SWAP_ADDRESS + address);
        } else {
            //this should be the case 99.99% of the time, the data is where we want it
            return sFLASH_ReadSingleByte(FLASH_STORAGE_ADDRESS + address);
        }
    }
    else {
        return 0xff;
    }
}

void HAL_EEPROM_Write(uint32_t address, uint8_t data)
{
    if (checkAddress(address)) {
        //first, read the entire buffer and stick it in the swap area for safe keeping
        uint8_t buf[USER_STORAGE_AVAILABLE + 1];
        buf[USER_STORAGE_AVAILABLE] = 1;
        sFLASH_ReadBuffer(buf, FLASH_STORAGE_ADDRESS, USER_STORAGE_AVAILABLE);
        sFLASH_WriteBuffer(buf, FLASH_STORAGE_SWAP_ADDRESS, USER_STORAGE_AVAILABLE);
        sFLASH_EraseSector(FLASH_STORAGE_SWAP_ADDRESS);

        //now, manipulate the data in RAM to store the new value
        buf[address] = data;

        //now erase the main sector and write the values back
        sFLASH_EraseSector(FLASH_STORAGE_ADDRESS);
        sFLASH_WriteBuffer(buf, FLASH_STORAGE_ADDRESS, sizeof(buf));

        //finally, erase the swap
        sFLASH_EraseSector(FLASH_STORAGE_SWAP_ADDRESS);
    }
}

void HAL_EEPROM_Get(uint32_t index, void *data, size_t length)
{
    if (checkAddress(index)) {
        if (sFLASH_ReadSingleByte(FLASH_STORAGE_ADDRESS + USER_STORAGE_AVAILABLE) != 1) {
            //in case of a power failure during a write, the data may be in swap
            sFLASH_ReadBuffer(data, FLASH_STORAGE_SWAP_ADDRESS + index, length);
        } else {
            //this should be the case 99.99% of the time, the data is where we want it
            sFLASH_ReadBuffer(data, FLASH_STORAGE_ADDRESS + index, length);
        }
    }
}

void HAL_EEPROM_Put(uint32_t index, const void *data, size_t length)
{
    if (checkAddress(index)) {
        //first, read the entire buffer and stick it in the swap area for safe keeping
        uint8_t buf[USER_STORAGE_AVAILABLE + 1];
        buf[USER_STORAGE_AVAILABLE] = 1;

        sFLASH_ReadBuffer(buf, FLASH_STORAGE_ADDRESS, USER_STORAGE_AVAILABLE);
        sFLASH_WriteBuffer(buf, FLASH_STORAGE_SWAP_ADDRESS, USER_STORAGE_AVAILABLE);
        sFLASH_EraseSector(FLASH_STORAGE_SWAP_ADDRESS);

        //now, manipulate the data in RAM to store the new value
        memcpy(buf + index, data, length);

        //now erase the main sector and write the values back
        sFLASH_EraseSector(FLASH_STORAGE_ADDRESS);
        sFLASH_WriteBuffer(buf, FLASH_STORAGE_ADDRESS, sizeof(buf));

        //finally, erase the swap
        sFLASH_EraseSector(FLASH_STORAGE_SWAP_ADDRESS);
    }
}

void HAL_EEPROM_Clear()
{
    sFLASH_EraseSector(FLASH_STORAGE_ADDRESS);
    sFLASH_EraseSector(FLASH_STORAGE_SWAP_ADDRESS);

}

bool HAL_EEPROM_Has_Pending_Erase()
{
    return false;
}

void HAL_EEPROM_Perform_Pending_Erase()
{
}
