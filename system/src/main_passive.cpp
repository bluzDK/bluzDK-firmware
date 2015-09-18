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

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static volatile uint32_t TimingLED;
static volatile uint32_t TimingIWDGReload;
static bool CLOUD_CONNECTED = false;

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : main.
 * Description    : main routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void app_setup_and_loop_passive(void)
{
    //Set some Particle flags to bypass functionality we don't need
    WLAN_SMART_CONFIG_START = 0;
    WLAN_SMART_CONFIG_STOP = 1;
    
    
    //setup all peripherals
    HAL_Core_Init();
    
    // We have running firmware, otherwise we wouldn't have gotten here
    DECLARE_SYS_HEALTH(ENTERED_Main);
    DEBUG("Hello from Spark!");
    
    //setup BLE stack
    HAL_Network_Init();
    
    //call user setup function, device may or may not be connected
    setup();
    
    while (1)
    {
        DECLARE_SYS_HEALTH(ENTERED_WLAN_Loop);
        
        //Execute user application loop
        DECLARE_SYS_HEALTH(ENTERED_Loop);
        if (system_mode()!=SAFE_MODE) {
            DEBUG("Entering User Loop");
            loop();
            DECLARE_SYS_HEALTH(RAN_Loop);
            DEBUG("Exited User Loop");
        }
        
        //we may not be connected. if not, don't try to manage anything cloud related
        if (HAL_Network_Connection()){
            if (CLOUD_CONNECTED) {
                DEBUG("Calling Spark Comm Loop");
                if (!Spark_Communication_Loop()) {
                    ERROR("Error when calling Spark Communication Loop");
                }
            } else {
                HAL_Delay_Milliseconds(2000);
                DEBUG("Calling Spark Connect");
                int err_code = Spark_Connect();
                if (err_code) {
                    ERROR("Error when calling Spark Connect");
                }
                HAL_Delay_Milliseconds(2000);
                Spark_Protocol_Init();
                DEBUG("Calling Spark Handshake");
                err_code = Spark_Handshake();
                if (err_code) {
                    ERROR("Error when calling Spark Handshake");
                }
                DEBUG("Handshake Complete");
                CLOUD_CONNECTED = true;
            }
        }
    }
}

