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

#include "app_scheduler.h"
#include "app_timer.h"
#include "app_timer_appsh.h"
#include "nrf_gpiote.h"
#include "softdevice_handler.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_wdt.h"
//#include "ble_conn_params.h"
#include "ble_scs.h"
#include "ble_advdata.h"
#include "device_manager.h"


#ifndef __NRF51_CONFIG_H
#define __NRF51_CONFIG_H

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */

#if PLATFORM_ID==103
#define APP_TIMER_MAX_TIMERS            11                                          /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         12                                          /**< Size of timer operation queues. */
#endif
#if PLATFORM_ID==269
#define APP_TIMER_MAX_TIMERS            2                                          /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                          /**< Size of timer operation queues. */
#endif


#define TIME_KEPPER_MILLISECONDS     	100                                         /**< Keep track of time in roughly 100 mSecond intervals. */
#define TIME_KEPPER_INTERVAL     		APP_TIMER_TICKS(TIME_KEPPER_MILLISECONDS, APP_TIMER_PRESCALER)     /**< Convert to clock ticks. */

#define APP_GPIOTE_MAX_USERS            1                                           /**< Maximum number of users of the GPIOTE handler. */

#define BUTTON_DETECTION_DELAY          APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)    /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */

#define IS_SRVC_CHANGED_CHARACT_PRESENT      0

#define APP_ADV_INTERVAL                160                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 100 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS      180                                         /**< The advertising timeout (in units of seconds). */
#define APP_ADV_NO_TIMEOUT              0                                           /**< Disables advertising timeout. */

#define SCHED_MAX_EVENT_DATA_SIZE       MAX(APP_TIMER_SCHED_EVT_SIZE, BLE_STACK_HANDLER_SCHED_EVT_SIZE)                   /**< Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#define SCHED_QUEUE_SIZE                10                                          /**< Maximum number of events in the scheduler queue. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(7.5, UNIT_1_25_MS)            /**< Minimum acceptable connection interval (0.5 seconds). */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(300, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (1 second). */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)     /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY    APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER)    /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT     3                                              /**< Number of attempts before giving up the connection parameter negotiation. */

#define SEC_PARAM_TIMEOUT               30                                          /**< Timeout for Pairing Request or Security Request (in seconds). */
#define SEC_PARAM_BOND                  1                                           /**< Perform bonding. */
#define SEC_PARAM_MITM                  0                                           /**< Man In The Middle protection not required. */
#define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE                        /**< No I/O capabilities. */
#define SEC_PARAM_OOB                   0                                           /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE          7                                           /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE          16                                          /**< Maximum encryption key size. */

#define BEACON_ADV_INTERVAL    			MSEC_TO_UNITS(950, UNIT_0_625_MS) /**< The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s). */
#define APP_BEACON_INFO_LENGTH          0x17                              /**< Total length of information advertised by the Beacon. */
#define APP_ADV_DATA_LENGTH             0x15                              /**< Length of manufacturer specific data in the advertisement. */
#define APP_DEVICE_TYPE                 0x02                              /**< 0x02 refers to Beacon. */
#define APP_MEASURED_RSSI               0xC4                              /**< The Beacon's measured RSSI at 1 meter distance in dBm. */
#define APP_COMPANY_IDENTIFIER          0x004C                            /**< Company identifier for Nordic Semiconductor ASA. as per www.bluetooth.org. */
#define APP_MAJOR_VALUE                 0x11, 0x22                        /**< Major value used to identify Beacons. */
#define APP_MINOR_VALUE                 0x33, 0x44                        /**< Minor value used to identify Beacons. */

#define EDDYSTONE_BEACON_LENGTH 0x18

#define EDDYSTONE_HEADER 0x02, 0x01, 0x06, 0x03, 0x03, 0xAA, 0xFE, 0x10, 0x16, 0xAA, 0xFE
#define EDDYSTONE_FRAME_TYPE 0x10
#define EDDYSTONE_TX_POWER 0xEB
#define EDDYSTONE_URL_SCHEME 0x02
#define EDDYSTONE_URL 0x6d, 0x63, 0x6f, 0x6e, 0x74, 0x69, 0x2e, 0x78, 0x79, 0x7a



//timers
app_timer_id_t millis_timer;
nrf_drv_timer_t micros_timer;

//wdt channel id
nrf_drv_wdt_channel_id m_channel_id;

//parameters
ble_gap_sec_params_t m_sec_params; /**< Security requirements for this application. */

//handles
scs_t m_scs;
uint16_t m_conn_handle; /**< Handle of the current connection. */

//system variables
uint32_t system_seconds;
uint32_t system_milliseconds;
uint32_t system_microseconds;
uint32_t system_connection_interval;

//cloud variables
bool isCloudConnected;
bool isCloudUpdating;

//device manager
dm_application_instance_t         m_app_handle;                                  /**< Application identifier allocated by device manager. */
dm_handle_t                       m_bonded_peer_handle;                          /**< Device reference handle to the current bonded central. */

#endif
