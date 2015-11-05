/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "bootloader_settings.h"
#include <stdint.h>
#include <dfu_types.h>

#if defined ( __CC_ARM )
uint8_t  m_boot_settings[CODE_PAGE_SIZE] __attribute__((at(BOOTLOADER_SETTINGS_ADDRESS))) __attribute__((used));              /**< This variable reserves a codepage for bootloader specific settings, to ensure the compiler doesn't locate any code or variables at his location. */
uint32_t m_uicr_bootloader_start_address __attribute__((at(NRF_UICR_BOOT_START_ADDRESS))) = BOOTLOADER_REGION_START;            /**< This variable ensures that the linker script will write the bootloader start address to the UICR register. This value will be written in the HEX file and thus written to UICR when the bootloader is flashed into the chip. */
#elif defined ( __GNUC__ )
__attribute__ ((section(".bootloaderSettings"))) uint8_t m_boot_settings[CODE_PAGE_SIZE];                                       /**< This variable reserves a codepage for bootloader specific settings, to ensure the compiler doesn't locate any code or variables at his location. */
__attribute__ ((section(".uicrBootStartAddress"))) volatile uint32_t m_uicr_bootloader_start_address = BOOTLOADER_REGION_START; /**< This variable ensures that the linker script will write the bootloader start address to the UICR register. This value will be written in the HEX file and thus written to UICR when the bootloader is flashed into the chip. */
#elif defined ( __ICCARM__ )
__no_init uint8_t m_boot_settings[CODE_PAGE_SIZE] @ 0x0003FC00;                                                                 /**< This variable reserves a codepage for bootloader specific settings, to ensure the compiler doesn't locate any code or variables at his location. */
__root    const uint32_t m_uicr_bootloader_start_address @ 0x10001014 = BOOTLOADER_REGION_START;                                /**< This variable ensures that the linker script will write the bootloader start address to the UICR register. This value will be written in the HEX file and thus written to UICR when the bootloader is flashed into the chip. */
#endif

#define SIZE_TEST_INDEX 8    /**< Index where first size field is located when half word aligned, or Bank Code 1 if word aligned. */
#define ZERO            0x00 /**< Value used for determine if struct is half word or word sized on first fields in the struct. */


void bootloader_util_settings_get(const bootloader_settings_t ** pp_bootloader_settings)
{
    static bootloader_settings_t s_converted_boot_settings;

    // Depending on the configuration / version the compiler used the enums used in bootloader
    // settings might have different padding (half word / word padded).
    // Half word padding would be, bc = Bank Code enum, crc (half word), xx = byte padding:
    // bc0 xx  crc crc - bc1 xx  xx  xx  - rest of data
    // Word padding would be, bc = Bank Code enum, crc (half word), xx = byte padding:
    // bc0 xx  xx  xx  - crc crc xx  xx  - bc1 xx  xx  xx - rest of data
    // To ensure full compability when updating from older bootloader to new, the padding must
    // be checked and conversion must be done accordingly.
    // In case of empty flash, 0xFFFFFFFF it is unimportant how settings are loaded.
    uint32_t pad_test = *((uint32_t *)&m_boot_settings[SIZE_TEST_INDEX]) & 0xFFFFFF00;

    if (pad_test == ZERO)
    {
        // Bank code enum is word sized, thus it is safe to cast without conversion.
        // Read only pointer to bootloader settings in flash.
        bootloader_settings_t const * const p_bootloader_settings =
            (bootloader_settings_t *)&m_boot_settings[0];

        *pp_bootloader_settings = p_bootloader_settings;
    }
    else
    {
        // Bank code enum is half word sized, thus crc is following the bank code in same word.
        uint32_t index                           = 0;
        s_converted_boot_settings.bank_0         =
            (bootloader_bank_code_t)((uint16_t)(m_boot_settings[index]) & 0xFFFF);

        index                                   += sizeof(uint16_t);
        s_converted_boot_settings.bank_0_crc     = uint16_decode(&m_boot_settings[index]);

        index                                   += sizeof(uint16_t);
        s_converted_boot_settings.bank_1         =
            (bootloader_bank_code_t)((uint16_t)(m_boot_settings[index]) & 0xFFFF);

        // Rest of settings are word sized values and thus will be aligned to next word.
        index                                   += sizeof(uint32_t);
        s_converted_boot_settings.bank_0_size    = uint32_decode(&m_boot_settings[index]);;

        index                                   += sizeof(uint32_t);
        s_converted_boot_settings.sd_image_size  = uint32_decode(&m_boot_settings[index]);;

        index                                   += sizeof(uint32_t);
        s_converted_boot_settings.bl_image_size  = uint32_decode(&m_boot_settings[index]);;

        index                                   += sizeof(uint32_t);
        s_converted_boot_settings.app_image_size = uint32_decode(&m_boot_settings[index]);;

        index                                   += sizeof(uint32_t);
        s_converted_boot_settings.sd_image_start = uint32_decode(&m_boot_settings[index]);;

        *pp_bootloader_settings = &s_converted_boot_settings;
    }
}
