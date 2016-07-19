/**
 ******************************************************************************
 * @file    main.cpp
 * @author  Satish Nair, Zachary Crockett, Zach Supalla and Mohit Bhoite
 * @version V1.0.0
 * @date    13-March-2013
 * 
 * Updated: 14-Feb-2014 David Sidrane <david_s5@usa.net>
 * 
 * @brief   Main program body.
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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
  ******************************************************************************
 */
  
/* Includes ------------------------------------------------------------------*/
#include "debug.h"
#include "system_mode.h"
#include "system_task.h"
#include "system_network.h"
#include "system_network_internal.h"
#include "core_hal.h"
#include "syshealth_hal.h"
#include "watchdog_hal.h"
#include "system_cloud.h"
#include "system_user.h"
#include "system_update.h"
#include "usb_hal.h"
#include "system_mode.h"
#include "rgbled.h"
#include "ledcontrol.h"
#include "delay_hal.h"
#include "timer_hal.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
volatile uint16_t ledOffTime = 2000, ledOnTime = 200;
static volatile uint32_t TimingLED;
static volatile uint32_t TimingIWDGReload;
static volatile uint32_t SystemSecondsTick;
static bool CLOUD_CONNECTED = false;
uint32_t on_mseconds = ledOnTime, off_mseconds = ledOnTime+ledOffTime;
uint16_t cloudErrors = 0;

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
extern "C" void HAL_SysTick_Handler(void) {
    uint32_t current_millis = HAL_Timer_Get_Milli_Seconds();
    if (!LED_RGB_IsOverRidden()) {
        if (current_millis > off_mseconds) {
            LED_On(LED_RGB);
            on_mseconds = ledOnTime+current_millis;
            off_mseconds = ledOnTime+ledOffTime+current_millis;
        } else if (current_millis > on_mseconds) {
            LED_Off(LED_RGB);
        }
    }

    //feed the dog
    if (TimingIWDGReload >= TIMING_IWDG_RELOAD)
    {
        TimingIWDGReload = 0;
        /* Reload WDG counter */
        HAL_Notify_WDT();
    }
    else
    {
        TimingIWDGReload+=HAL_Get_Sys_Tick_Interval();
    }

    //tick the system seconds (separate from millis() so it won't roll over after 49 days)
    if (SystemSecondsTick >= 1000)
    {
        SystemSecondsTick = 0;
        /* Reload WDG counter */
        HAL_Tick_System_Seconds();
    }
    else
    {
        SystemSecondsTick+=HAL_Get_Sys_Tick_Interval();
    }

    //check on system updates and reset if necessary
    if(SPARK_FLASH_UPDATE)
    {
        if (TimingFlashUpdateTimeout >= TIMING_FLASH_UPDATE_TIMEOUT)
        {
            //Reset is the only way now to recover from stuck OTA update
            HAL_Core_System_Reset();
        }
        else
        {
            TimingFlashUpdateTimeout+=HAL_Get_Sys_Tick_Interval();
        }
    }


}

//stubs
uint16_t system_button_pushed_duration(uint8_t button, void*)
{
    return 0;
}

extern "C" void system_part1_post_init() __attribute__((weak));

// this is overridden for modular firmware
void system_part1_post_init()
{
}

void platform_event_callback(uint8_t event, uint8_t *data, uint16_t length)
{
    //set mode function
    if (event == 1) {
        if (length == 1 && data[0] == 1) {
            set_system_mode(MANUAL);
        } else {
            set_system_mode(AUTOMATIC);
        }
    }
}

/*******************************************************************************
 * Function Name  : main.
 * Description    : main routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void app_setup_and_loop_passive(void)
{
    system_part1_post_init();

    // Register for events that may come from the platform
    HAL_Register_Platform_Events(platform_event_callback);
    
    //Set some Particle flags to bypass functionality we don't need
    SPARK_CLOUD_SOCKETED = 0;
    SPARK_CLOUD_CONNECTED = 0;
    HAL_Set_Cloud_Connection(false);

    //setup all peripherals
    HAL_Core_Init();
    
    // We have running firmware, otherwise we wouldn't have gotten here
    DECLARE_SYS_HEALTH(ENTERED_Main);
    DEBUG("Hello from Spark!");
    
    //setup BLE stack
    HAL_Network_Init();
    
    LED_SetRGBColor(system_mode()==SAFE_MODE ? RGB_COLOR_YELLOW : RGB_COLOR_GREEN);
    LED_On(LED_RGB);

    //initialize the spark protocol
    Spark_Protocol_Init();

    //call user setup function, device may or may not be connected
    if (system_mode()!=SAFE_MODE) {
        setup();
    }

    while (1)
    {
        DECLARE_SYS_HEALTH(ENTERED_WLAN_Loop);

        //Execute user application loop
        DECLARE_SYS_HEALTH(ENTERED_Loop);
        if (system_mode()!=SAFE_MODE && !SPARK_FLASH_UPDATE) {
//            DEBUG("Entering User Loop");
            loop();
            DECLARE_SYS_HEALTH(RAN_Loop);
//            DEBUG("Exited User Loop");
        }

        if (!SPARK_FLASH_UPDATE) {
            HAL_Loop_Iteration();
        }
        
        //we may not be connected. if not, don't try to manage anything cloud related
        if (system_mode()!=MANUAL) {
            if (HAL_Network_Connection()) {
                if (CLOUD_CONNECTED) {
//                  DEBUG("Calling Spark Comm Loop");
//                  Spark_Process_Events();
                    if (!Spark_Communication_Loop()) {
                        cloudErrors++;
                        ERROR("Error when calling Spark Comm Loop");
                        if (cloudErrors > 2) {
                          cloudErrors = 0;
                          HAL_Handle_Cloud_Disconnect();
                        }
                    }
                } else {
                    ledOffTime = 250;
                    HAL_Delay_Milliseconds(2000);
                    DEBUG("Calling Spark Connect");
                    int err_code = spark_cloud_socket_connect();
                    if (err_code) {
                        ERROR("Error when calling Spark Connect");
                    }
                    SPARK_CLOUD_SOCKETED = 1;

                    HAL_Delay_Milliseconds(2000);
                    DEBUG("Calling Spark Handshake");
                    err_code = Spark_Handshake(false);
                    if (err_code) {
                        ERROR("Error when calling Spark Handshake");
                        SPARK_CLOUD_SOCKETED = 0;
                        ledOffTime = 2000;
                        HAL_Handle_Cloud_Disconnect();
                    } else {
                        LED_SetRGBColor(system_mode()==SAFE_MODE ? RGB_COLOR_YELLOW : RGB_COLOR_CYAN);
                        DEBUG("Handshake Complete");

                        CLOUD_CONNECTED = true;
                        SPARK_CLOUD_CONNECTED = 1;
                        ledOffTime = 2000;
                        HAL_Set_Cloud_Connection(true);
                    }
                }
            } else {
                if (CLOUD_CONNECTED) {
                    DEBUG("Connection Lost");
                    //we disconnected
                    CLOUD_CONNECTED = false;

                    SPARK_CLOUD_SOCKETED = 0;
                    SPARK_CLOUD_CONNECTED = 0;
                    HAL_Set_Cloud_Connection(false);

                    ledOffTime = 2000;
                    LED_SetRGBColor(system_mode()==SAFE_MODE ? RGB_COLOR_YELLOW : RGB_COLOR_GREEN);

                }
                if (!HAL_Is_Advertising()) {
                    ledOffTime = 2000;
                    LED_SetRGBColor(RGB_COLOR_BLUE);
                } else {
                    ledOffTime = 2000;
    //                LED_SetRGBColor(RGB_COLOR_GREEN);
                }
            }
        } else {
            LED_SetRGBColor(HAL_Network_Connection() ? RGB_COLOR_WHITE : RGB_COLOR_GREEN);
        }
    }
}

