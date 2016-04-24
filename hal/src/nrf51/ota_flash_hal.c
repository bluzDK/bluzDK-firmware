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

#include "flash.h"
#include "ota_flash_hal.h"
#undef STATIC_ASSERT
#include "hw_config.h"
#include "sst25vf_spi.h"

#if MODULAR_FIRMWARE

#if PLATFORM_ID==103   /*--bluz*/
const module_bounds_t module_bootloader = { 0x4000, 0x3C000, 0x40000, MODULE_FUNCTION_BOOTLOADER, 0, MODULE_STORE_MAIN };
const module_bounds_t module_system_part1 = { 0x1F000, 0x18000, 0x37000, MODULE_FUNCTION_SYSTEM_PART, 1, MODULE_STORE_MAIN };
const module_bounds_t module_user = { 0x5000, 0x37000, 0x3C000, MODULE_FUNCTION_USER_PART, 2, MODULE_STORE_MAIN};
const module_bounds_t module_factory = { 0x1F000, 0x1021000, 0x1040000, MODULE_FUNCTION_USER_PART, 1, MODULE_STORE_FACTORY};
const module_bounds_t* module_bounds[] = { &module_bootloader, &module_system_part1, &module_user, &module_factory };
const module_bounds_t module_ota = { 0x1D000, 0x1004000, 0x1021000, MODULE_FUNCTION_NONE, 0, MODULE_STORE_SCRATCHPAD};
#endif

#if PLATFORM_ID==269   /*--bluz-gw*/
const module_bounds_t module_bootloader = { 0x4000, 0x3C000, 0x40000, MODULE_FUNCTION_BOOTLOADER, 0, MODULE_STORE_MAIN };
const module_bounds_t module_system_part1 = { 0x1A000, 0x1D000, 0x37000, MODULE_FUNCTION_SYSTEM_PART, 1, MODULE_STORE_MAIN };
const module_bounds_t module_user = { 0x5000, 0x37000, 0x3C000, MODULE_FUNCTION_USER_PART, 2, MODULE_STORE_MAIN};
const module_bounds_t module_factory = { 0x1F000, 0x1021000, 0x1040000, MODULE_FUNCTION_USER_PART, 1, MODULE_STORE_FACTORY};
const module_bounds_t* module_bounds[] = { &module_bootloader, &module_system_part1, &module_user, &module_factory };
const module_bounds_t module_ota = { 0x1D000, 0x1004000, 0x1021000, MODULE_FUNCTION_NONE, 0, MODULE_STORE_SCRATCHPAD};
#endif

#else
const module_bounds_t module_bootloader = { 0x4000, 0x3C000, 0x40000, MODULE_FUNCTION_BOOTLOADER, 0, MODULE_STORE_MAIN};
const module_bounds_t module_user = { 0x24000, 0x18000, 0x3C000, MODULE_FUNCTION_MONO_FIRMWARE, 0, MODULE_STORE_MAIN};
const module_bounds_t module_factory = { 0x1F000, 0x1021000, 0x1040000, MODULE_FUNCTION_MONO_FIRMWARE, 0, MODULE_STORE_FACTORY};
const module_bounds_t* module_bounds[] = { &module_bootloader, &module_user, &module_factory };

const module_bounds_t module_ota = { 0x1D000, 0x1004000, 0x1021000, MODULE_FUNCTION_NONE, 0, MODULE_STORE_SCRATCHPAD};
#endif

const unsigned module_bounds_length = 3;
void HAL_OTA_Add_System_Info(hal_system_info_t* info, bool create, void* reserved);

/**
 * Finds the location where a given module is stored. The module is identified
 * by it's funciton and index.
 * @param module_function   The function of the module to find.
 * @param module_index      The function index of the module to find.
 * @return the module_bounds corresponding to the module, NULL when not found.
 */
const module_bounds_t* find_module_bounds(uint8_t module_function, uint8_t module_index)
{
    for (unsigned i=0; i<module_bounds_length; i++) {
        if (module_bounds[i]->module_function==module_function && module_bounds[i]->module_index==module_index)
            return module_bounds[i];
    }
    return NULL;
}

/**
 * Determines if a given address is in range.
 * @param test      The address to test
 * @param start     The start range (inclusive)
 * @param end       The end range (inclusive)
 * @return {@code true} if the address is within range.
 */
inline bool in_range(uint32_t test, uint32_t start, uint32_t end)
{
    return test>=start && test<=end;
}


/**
 * Find the module_info at a given address. No validation is done so the data
 * pointed to should not be trusted.
 * @param bounds
 * @return
 */
const module_info_t* locate_module(const module_bounds_t* bounds) {
    return FLASH_ModuleInfo(FLASH_INTERNAL, bounds->start_address);
}

bool validate_module_dependencies(const module_bounds_t* bounds, bool userOptional)
{
    bool valid = false;
    const module_info_t* module = locate_module(bounds);
    if (module)
    {
        if (module->dependency.module_function == MODULE_FUNCTION_NONE || (userOptional && module_function(module)==MODULE_FUNCTION_USER_PART)) {
            valid = true;
        }
        else {
            // deliberately not transitive, so we only check the first dependency
            // so only user->system_part_2 is checked
            const module_bounds_t* dependency_bounds = find_module_bounds(module->dependency.module_function, module->dependency.module_index);
            const module_info_t* dependency = locate_module(dependency_bounds);
            valid = dependency && (dependency->module_version>=module->dependency.module_version);
        }
    }
    return valid;
}

bool HAL_Verify_User_Dependencies()
{
    const module_bounds_t* bounds = find_module_bounds(MODULE_FUNCTION_USER_PART, 2);
    return validate_module_dependencies(bounds, false);
}

/**
 * Fetches and validates the module info found at a given location.
 * @param target        Receives the module into
 * @param bounds        The location where to retrieve the module from.
 * @param userDepsOptional
 * @return {@code true} if the module info can be read via the info, crc, suffix pointers.
 */
bool fetch_module(hal_module_t* target, const module_bounds_t* bounds, bool userDepsOptional, uint16_t check_flags)
{
    memset(target, 0, sizeof(*target));
    
    target->bounds = *bounds;
    if (NULL!=(target->info = locate_module(bounds)))
    {
        target->validity_checked = MODULE_VALIDATION_RANGE | MODULE_VALIDATION_DEPENDENCIES | MODULE_VALIDATION_PLATFORM | check_flags;
        target->validity_result = 0;
        const uint8_t* module_end = (const uint8_t*)target->info->module_end_address;
        const module_bounds_t* expected_bounds = find_module_bounds(module_function(target->info), module_index(target->info));
        if (expected_bounds && in_range((uint32_t)module_end, expected_bounds->start_address, expected_bounds->end_address)) {
            target->validity_result |= MODULE_VALIDATION_RANGE;
            target->validity_result |= (PLATFORM_ID==module_platform_id(target->info)) ? MODULE_VALIDATION_PLATFORM : 0;
            // the suffix ends at module_end, and the crc starts after module end
            target->crc = (module_info_crc_t*)module_end;
            target->suffix = (module_info_suffix_t*)(module_end-sizeof(module_info_suffix_t));
            if (validate_module_dependencies(bounds, userDepsOptional))
                target->validity_result |= MODULE_VALIDATION_DEPENDENCIES;
            if ((target->validity_checked & MODULE_VALIDATION_INTEGRITY) && FLASH_VerifyCRC32(FLASH_INTERNAL, bounds->start_address, module_length(target->info)))
                target->validity_result |= MODULE_VALIDATION_INTEGRITY;
        }
        else
            target->info = NULL;
    }
    return target->info!=NULL;
}


void HAL_System_Info(hal_system_info_t* info, bool construct, void* reserved)
{
    if (construct) {
        info->platform_id = PLATFORM_ID;
        // bootloader, system 1, system 2, optional user code and factory restore
        uint8_t count = module_bounds_length;
        info->modules = malloc(module_bounds_length * sizeof(hal_module_t));
        if (info->modules) {
            info->module_count = count;
            for (unsigned i=0; i<module_bounds_length; i++) {
                fetch_module(info->modules+i, module_bounds[i], false, MODULE_VALIDATION_INTEGRITY);
            }
        }
    }
    else
    {
        free(info->modules);
        info->modules = NULL;
    }
    HAL_OTA_Add_System_Info(info, construct, reserved);
    
}

void HAL_OTA_Add_System_Info(hal_system_info_t* info, bool create, void* reserved)
{
    // presently no additional key/value pairs to send back
    info->key_values = NULL;
    info->key_value_count = 0;
}

uint32_t HAL_OTA_FlashAddress()
{
    return FLASH_FW_ADDRESS;
}

uint32_t HAL_OTA_FlashLength()
{
    return (int32_t)(FLASH_LENGTH - FLASH_FW_ADDRESS);
}

uint16_t HAL_OTA_ChunkSize()
{
    return 512;
}

uint16_t HAL_OTA_SessionTimeout()
{
    return 0;
}

bool HAL_FLASH_Begin(uint32_t address, uint32_t length, void* reserved)
{
    FLASH_Begin(address, length);
    return true;
}

int HAL_FLASH_Update(const uint8_t *pBuffer, uint32_t address, uint32_t length, void* reserved)
{
    return FLASH_Update(pBuffer, address, length);
}

hal_update_complete_t HAL_FLASH_End(void* reserved)
{
    FLASH_End();
    return HAL_UPDATE_APPLIED_PENDING_RESTART;
}

void HAL_FLASH_Read_ServerAddress(ServerAddress* server_addr)
{
}


bool HAL_OTA_Flashed_GetStatus(void)
{
    return false;
}

void HAL_OTA_Flashed_ResetStatus(void)
{
}

void HAL_FLASH_Read_ServerPublicKey(uint8_t *keyBuffer)
{
    sFLASH_ReadBuffer(keyBuffer, FLASH_PUBLIC_KEY, 294);
}

int HAL_FLASH_Read_CorePrivateKey(uint8_t *keyBuffer, private_key_generation_t* generation)
{
    sFLASH_ReadBuffer(keyBuffer, FLASH_PRIVATE_KEY, 612);
    return EXTERNAL_FLASH_CORE_PRIVATE_KEY_LENGTH;
}

uint16_t HAL_Set_Claim_Code(const char* code)
{
    return -1;
}

uint16_t HAL_Get_Claim_Code(char* buffer, unsigned len)
{
    if (len)
        buffer[0] = 0;
    return 0;
}


