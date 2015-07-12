/**
 ******************************************************************************
 * @file    nrf_hw_config.h
 * @author  Eric Ely, Satish Nair, Zachary Crockett and Mohit Bhoite
 * @version V1.0.0
 * @date    8-October-2014
 * @brief   Hardware Configuration & Setup. Modified from Spark FW
 ******************************************************************************
  Copyright (c) 2013 Spark Labs, Inc.  All rights reserved.

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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NRF_HW_CONFIG_H
#define __NRF_HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "spi_master.h"
#include "nrf_gpio.h"
#include "app_util_platform.h"
#include "system_tick_hal.h"
#include "nrf51_config.h"
#include "nrf51_callbacks.h"

#define SPI_MASTER_1_ENABLE				1
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP

//Board LED's and buttons
#define BOARD_LED_PIN          			20                                       /**< Main LED on the board. */
#define RGB_LED_PIN_NO_1          		19                                       /**< Pin one for the RGB LED. */
#define RGB_LED_PIN_NO_2          		18                                       /**< Pin one for the RGB LED. */
#define RGB_LED_PIN_NO_3          		17                                       /**< Pin one for the RGB LED. */
#define BOARD_BUTTON          			7                                        /**< Main LED on the board. */

//Flash SPI address
#define SPIM1_SCK_PIN       24u     /**< SPI clock GPIO pin number. */
#define SPIM1_MOSI_PIN      23u     /**< SPI Master Out Slave In GPIO pin number. */
#define SPIM1_MISO_PIN      22u     /**< SPI Master In Slave Out GPIO pin number. */
#define SPIM1_SS_PIN        21u     /**< SPI Slave Select GPIO pin number. */

//UART address
#define RX_PIN_NUMBER  11
#define TX_PIN_NUMBER  9
#define CTS_PIN_NUMBER 10
#define RTS_PIN_NUMBER 8
#define HWFC           true

//FLash addresses
#define FLASH_FW_STATUS 0x0100
#define FLASH_FW_LENGTH1 0x0101
#define FLASH_FW_LENGTH2 0x0102
#define FLASH_FW_LENGTH3 0x0103

#define FLASH_FW_ADDRESS 0x01A000
#define FLASH_LENGTH 0x040000

//On-Board Flash Addresses
#define CORE_FW_ADDRESS			        ((uint32_t)0x08005000)

//HW Init Functions
void leds_init(void);
void timers_init(void);
void gpiote_init(void);
void buttons_init(void);

//BLE Stack Functions
void ble_stack_init(void);
void scheduler_init(void);
void gap_params_init(void);
void services_init(void);
void advertising_init(void);
void conn_params_init(void);
void sec_params_init(void);
void advertising_start(void);
void timers_start(void);

//event handling
void power_manage(void);
void app_sched_execute(void);


//Flash functions
uint32_t OTA_FlashAddress(void);
uint32_t OTA_FlashLength(void);
void FLASH_Begin(uint32_t sFLASH_Address, uint32_t fileSize);
uint32_t FLASH_PagesMask(uint32_t fileSize);
uint16_t FLASH_Update(uint8_t *pBuffer, uint32_t bufferSize);
void FLASH_End(void);

/* Exported functions ------------------------------------------------------- */
void Set_System(void);
void NVIC_Configuration(void);
void SysTick_Configuration(void);
void SysTick_Disable(void);
void System1MsTick(void);
system_tick_t GetSystem1MsTick(void);

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


#endif  /*__NRF_HW_CONFIG_H*/
