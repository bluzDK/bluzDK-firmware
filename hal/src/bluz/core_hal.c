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

#include "core_hal.h"
#include "hw_config.h"
#include "data_services.h"
#include "nrf51_config.h"
#undef STATIC_ASSERT
#include "flash.h"
#include "gpio_hal.h"
#include "rgbled.h"
#include "ota_flash_hal.h"

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

void HAL_Core_Init(void)
{
    isCloudUpdating =  false;
    system_init();
    leds_init();
    timers_init();
    gpiote_init();
    buttons_init();
    wdt_init();
}

void HAL_Network_Init(void)
{
    ble_stack_init();
    scheduler_init();
    device_manager_init();
    gap_params_init();
    services_init();
    advertising_init();
    data_service_init();
    conn_params_init();
    external_flash_init();
    
    timers_start();
    advertising_start();
}

void HAL_Handle_Cloud_Disconnect(void)
{
    ble_disconnect();
}

void HAL_Loop_Iteration(void)
{
    
}

void HAL_Set_Cloud_Connection(bool connected)
{
    set_cloud_connection_state(connected);
}

void HAL_Events_Manage(void)
{
}

bool HAL_Network_Connection(void)
{
    return state == BLE_CONNECTED;
}

bool HAL_Is_Advertising(void)
{
    return state == BLE_ADVERTISING;
}

/*******************************************************************************
 * Function Name  : HAL_Core_Config.
 * Description    : Called in startup routine, before calling C++ constructors.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void HAL_Core_Config(void)
{
    HAL_Pin_Mode(D7, OUTPUT);
    HAL_GPIO_Write(D7, 1);
}

bool HAL_Core_Mode_Button_Pressed(uint16_t pressedMillisDuration)
{
    return false;
}

void HAL_Core_Mode_Button_Reset(void)
{    
}

void HAL_Core_System_Reset(void)
{  
}

void HAL_Core_Factory_Reset(void)
{
}

void HAL_Core_Enter_Bootloader(bool persist)
{
}

void HAL_Core_Enter_Stop_Mode(uint16_t wakeUpPin, uint16_t edgeTriggerMode, long seconds)
{
}

void HAL_Core_Execute_Stop_Mode(void)
{
}

void HAL_Core_Enter_Standby_Mode(void)
{
    power_manage();
    app_sched_execute();
}

void HAL_Core_CPU_Sleep(void)
{
    power_manage();
    app_sched_execute();
}

void HAL_Core_Execute_Standby_Mode(void)
{
}

void HAL_Core_Enter_Safe_Mode(void* reserved)
{
}

uint32_t HAL_Get_Sys_Tick_Interval(void)
{
    return TIME_KEPPER_MILLISECONDS;
}
/**
 * @brief  Computes the 32-bit CRC of a given buffer of byte data.
 * @param  pBuffer: pointer to the buffer containing the data to be computed
 * @param  BufferSize: Size of the buffer to be computed
 * @retval 32-bit CRC
 */
uint32_t HAL_Core_Compute_CRC32(const uint8_t *pBuffer, uint32_t bufferSize)
{
    return Compute_CRC32(0, pBuffer, bufferSize);
}

// todo find a technique that allows accessor functions to be inlined while still keeping
// hardware independence.
bool HAL_watchdog_reset_flagged() 
{
    return false;
}

void HAL_Notify_WDT()
{
    wtd_feed();
}

void CallConstructors(void);
//main function
int main(void) {
    CallConstructors();
    app_setup_and_loop_passive();
    return 0;
}

//Particle State Functions
uint32_t HAL_Core_Runtime_Info(runtime_info_t* info, void* reserved)
{
    return 0;
}

int HAL_Feature_Set(HAL_Feature feature, bool enabled)
{
    return -1;
}

bool HAL_Feature_Get(HAL_Feature feature)
{
    return false;
}

int HAL_Set_System_Config(hal_system_config_t config_item, const void* data, unsigned length)
{
    return -1;
}

void HAL_Bootloader_Lock(bool lock)
{
    if (lock)
        FLASH_WriteProtection_Enable(BOOTLOADER_FLASH_PAGES);
    else
        FLASH_WriteProtection_Disable(BOOTLOADER_FLASH_PAGES);
}

uint16_t HAL_Bootloader_Get_Flag(BootloaderFlag flag)
{
    switch (flag) {
        case BOOTLOADER_FLAG_VERSION:
            break;
        case BOOTLOADER_FLAG_STARTUP_MODE:
            break;
    }
    return 0;
}

bool HAL_Core_System_Reset_FlagSet(RESET_TypeDef resetType)
{
    
    switch(resetType)
    {
        case PIN_RESET:
            break;
            
        case SOFTWARE_RESET:
            break;
            
        case WATCHDOG_RESET:
            break;
            
        case LOW_POWER_RESET:
            break;
    }
    
    return true;
}

bool HAL_Core_Validate_User_Module(void)
{
    bool valid = false;
    
    //CRC verification Enabled by default
    if (FLASH_isUserModuleInfoValid(FLASH_INTERNAL, USER_FIRMWARE_IMAGE_LOCATION, USER_FIRMWARE_IMAGE_LOCATION))
    {
        //CRC check the user module and set to module_user_part_validated
//        valid = FLASH_VerifyCRC32(FLASH_INTERNAL, USER_FIRMWARE_IMAGE_LOCATION,
//                                  FLASH_ModuleLength(FLASH_INTERNAL, USER_FIRMWARE_IMAGE_LOCATION))
//        && HAL_Verify_User_Dependencies();
        
        valid = HAL_Verify_User_Dependencies();
    }
//    else if(FLASH_isUserModuleInfoValid(FLASH_INTERNAL, INTERNAL_FLASH_FAC_ADDRESS, USER_FIRMWARE_IMAGE_LOCATION))
//    {
//        //Reset and let bootloader perform the user module factory reset
//        //Doing this instead of calling FLASH_RestoreFromFactoryResetModuleSlot()
//        //saves precious system_part2 flash size i.e. fits in < 128KB
////        HAL_Core_Factory_Reset();
////        
////        while(1);//Device should reset before reaching this line
//    }
    
    return valid;
}

void HAL_Core_Write_Backup_Register(uint32_t BKP_DR, uint32_t Data)
{
}
uint32_t HAL_Core_Read_Backup_Register(uint32_t BKP_DR)
{
    return 0xFFFFFFFF;
}