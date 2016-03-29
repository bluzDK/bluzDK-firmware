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
#ifndef __NRF_HW_CONFIG_H
#define __NRF_HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "nrf51_config.h"
#include "nrf51_callbacks.h"
#include "spi_master.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "app_util_platform.h"
#include "system_tick_hal.h"
#include "hw_layout.h"

//On-Board Flash Addresses
#define CORE_FW_ADDRESS			        ((uint32_t)0x08005000)

//BLE State Information
typedef enum {
    BLE_OFF,
    BLE_ADVERTISING,
    BLE_SLEEPING,
    BLE_CONNECTED,
} BLE_STATE;

volatile BLE_STATE state;

//HW Init Functions
void system_init(void);
void leds_init(void);
void timers_init(void);
void gpiote_init(void);
void buttons_init(void);
void external_flash_init(void);
void gap_params_init(void);
void device_manager_init(void);
void scheduler_init(void);

//BLE Stack Functions
void ble_stack_init(void);
void scheduler_init(void);
void gap_params_init(void);
void services_init(void);
void advertising_init(void);
void conn_params_init(void);
void sec_params_init(void);
void advertising_start(void);
void advertising_stop(void);
void ble_disconnect(void);
uint32_t timers_start(void);
uint32_t timers_stop(void);

int register_radio_callback(void (*radio_callback)(bool radio_active));

//Data Services Functions
void data_service_init(void);

//event handling
void power_manage(void);
void app_sched_execute(void);

//useful functions
void blinkLED(int times);
void heartBeat(void);
uint32_t system_millis(void);
uint32_t system_micros(void);
void set_cloud_connection_state(bool connected);

//Flash functions
uint16_t FLASH_GetDeviceInt(void);
uint32_t OTA_FlashAddress(void);
uint32_t OTA_FlashLength(void);
void FLASH_Begin(uint32_t sFLASH_Address, uint32_t fileSize);
uint32_t FLASH_PagesMask(uint32_t fileSize);
uint16_t FLASH_Update(const uint8_t *pBuffer, uint32_t address, uint32_t bufferSize);
void FLASH_End(void);

/* Exported functions ------------------------------------------------------- */
void Set_System(void);
void NVIC_Configuration(void);
void SysTick_Configuration(void);
void SysTick_Disable(void);
void System1MsTick(void);
system_tick_t GetSystem1MsTick(void);
uint32_t Compute_CRC32(uint32_t crc, const uint8_t* buf, size_t size);

void IWDG_Reset_Enable(uint32_t msTimeout);

#define SYSTEM_FLAG(x) (x)
void Load_SystemFlags(void);
void Save_SystemFlags(void);

void Bootloader_Update_Version(uint16_t bootloaderVersion);

/* External variables --------------------------------------------------------*/
extern uint8_t USE_SYSTEM_FLAGS;
extern uint16_t Bootloader_Version_SysFlag;
extern uint16_t NVMEM_SPARK_Reset_SysFlag;
extern uint16_t FLASH_OTA_Update_SysFlag;
extern uint16_t OTA_FLASHED_Status_SysFlag;
extern uint16_t Factory_Reset_SysFlag;
extern uint8_t dfu_on_no_firmware;
extern uint8_t Factory_Reset_Done_SysFlag;
extern uint8_t StartupMode_SysFlag;
extern uint32_t RCC_CSR_SysFlag;

void Save_Reset_Syndrome();

void FLASH_WriteProtection_Enable(uint32_t FLASH_Sectors);
void FLASH_WriteProtection_Disable(uint32_t FLASH_Sectors);


#endif  /*__NRF_HW_CONFIG_H*/
