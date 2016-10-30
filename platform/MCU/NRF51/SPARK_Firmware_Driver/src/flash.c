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
#include "rgbled_hal.h"
#include "pstorage.h"

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

/**@brief Function for handling callbacks from pstorage module.
 *
 * @details Handles pstorage results for clear and storage operation. For detailed description of
 *          the parameters provided with the callback, please refer to \ref pstorage_ntf_cb_t.
 */
bool stillWorking = false;
static void pstorage_callback_handler(pstorage_handle_t * p_handle,
                                      uint8_t             op_code,
                                      uint32_t            result,
                                      uint8_t           * p_data,
                                      uint32_t            data_len)
{
    switch (op_code)
    {
        case PSTORAGE_STORE_OP_CODE:
            
            break;
            
        case PSTORAGE_CLEAR_OP_CODE:
            
            if (result == NRF_SUCCESS)
            {
                stillWorking = false;
                return;
            }
            else
            {
                stillWorking = false;
                return;
            }
            break;
            
        default:
            break;
    }
    APP_ERROR_CHECK(result);
}

bool FLASH_CopyFW(uint32_t flashFWLocation, uint32_t fw_len, bool wipeUserApp, bool bootloader)
{
    uint32_t         err_code;
    
    Set_RGB_LED_Values(0,0,255);
    
    //let's init the pstorage
    pstorage_handle_t m_storage_handle_app;
    pstorage_module_param_t storage_module_param = {.cb = pstorage_callback_handler};

    if (wipeUserApp) {
        //hack for now...
        fw_len += 0x4000;
    }
    storage_module_param.block_size = 0x100;
    storage_module_param.block_count = fw_len / 256;
    
    err_code = pstorage_raw_register(&storage_module_param, &m_storage_handle_app);
    APP_ERROR_CHECK(err_code);
    
    const module_info_t* modinfo = FLASH_ModuleInfo(FLASH_SERIAL, flashFWLocation);
    if (!bootloader && modinfo->module_function == MODULE_FUNCTION_BOOTLOADER) {
        return false;
    }
    
    m_storage_handle_app.block_id  = (uint32_t)modinfo->module_start_address;

    if (!FLASH_isUserModuleInfoValid(FLASH_SERIAL, flashFWLocation, 0x00)) {
        return false;
    }
    uint32_t    ops_count = 7;
    //now clear the nrf51 flash
    stillWorking = true;
    err_code = pstorage_raw_clear(&m_storage_handle_app, fw_len);
    do {
        app_sched_execute();
        err_code = pstorage_access_status_get(&ops_count);
    }
    while(ops_count != 0);
    
    app_sched_execute();
    APP_ERROR_CHECK(err_code);
    //now read from SPI Flash one page at a time and copy over to internal flash
    uint8_t buf[PSTORAGE_FLASH_PAGE_SIZE];
    uint32_t addr = flashFWLocation;
    for (int i = 0; i < fw_len; i+=PSTORAGE_FLASH_PAGE_SIZE) {
        sFLASH_ReadBuffer(buf, addr, PSTORAGE_FLASH_PAGE_SIZE);
        err_code = pstorage_raw_store(&m_storage_handle_app,
                                      (uint8_t *)buf,
                                      PSTORAGE_FLASH_PAGE_SIZE,
                                      i);
        APP_ERROR_CHECK(err_code);
        
        do {
            app_sched_execute();
            pstorage_access_status_get(&ops_count);
        }
        while(ops_count != 0);
        
        
        addr+=PSTORAGE_FLASH_PAGE_SIZE;
    }
    sFLASH_EraseSector(FLASH_FW_STATUS);
    sFLASH_WriteSingleByte(FLASH_FW_STATUS, 0x00);
    Set_RGB_LED_Values(0,0,0);
    return true;
}

//Will delete the user app located at the user firmware location
bool FLASH_WipeUserApp()
{
    uint32_t         err_code;
    //only need to wipe out the first 4k since that is what holds the module info
    uint32_t         fw_len = 0x1000;

    Set_RGB_LED_Values(0,0,255);

    //let's init the pstorage
    pstorage_handle_t m_storage_handle_app;
    pstorage_module_param_t storage_module_param = {.cb = pstorage_callback_handler};

    storage_module_param.block_size = 0x100;
    storage_module_param.block_count = fw_len / 256;

    err_code = pstorage_raw_register(&storage_module_param, &m_storage_handle_app);
    APP_ERROR_CHECK(err_code);

    m_storage_handle_app.block_id  = USER_FIRMWARE_IMAGE_LOCATION;

    uint32_t    ops_count = 7;
    //now clear the nrf51 flash
    stillWorking = true;
    err_code = pstorage_raw_clear(&m_storage_handle_app, fw_len);
    do {
        app_sched_execute();
        err_code = pstorage_access_status_get(&ops_count);
    }
    while(ops_count != 0);

    app_sched_execute();
    APP_ERROR_CHECK(err_code);
    Set_RGB_LED_Values(0,0,0);
    return true;
}
