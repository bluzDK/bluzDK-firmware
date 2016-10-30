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
#ifndef __FLASH_H
#define __FLASH_H

#include <stdbool.h>
#include "hw_layout.h"
#include "flash_device_hal.h"
#include "module_info.h"
#include "module_info_hal.h"

bool FLASH_isUserModuleInfoValid(uint8_t flashDeviceID, uint32_t startAddress, uint32_t expectedAddress);
const module_info_t* FLASH_ModuleInfo(uint8_t flashDeviceID, uint32_t startAddress);
bool FLASH_VerifyCRC32(uint8_t flashDeviceID, uint32_t startAddress, uint32_t length);
uint32_t FLASH_ModuleLength(uint8_t flashDeviceID, uint32_t startAddress);
bool FLASH_CopyFW(uint32_t flashFWLocation, uint32_t fw_len, bool wipeUserApp, bool bootloader);
bool FLASH_WipeUserApp();

#endif  /*__FLASH_H*/
