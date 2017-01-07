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

#include <string.h>
#include "module_info.h"
#include "flash.h"
#undef STATIC_ASSERT
#include "hw_config.h"
#include "sst25vf_spi.h"
#include "data_management_layer.h"
#include "socket_manager.h"
#include "debug.h"
#include "rgbled.h"
#include "device_manager.h"
#include "pstorage.h"
#include "ble_radio_notification.h"
#include "ble_conn_params.h"
#include "ble_hci.h"
#include "custom_data_service.h"
#include "info_data_service.h"
#include "nrf_drv_wdt.h"
#include "client_handling.h"
#include "app_timer.h"
#include "spi_master_fast.h"
#include "beacon_helper.h"

uint32_t NbrOfPage = 0;
uint16_t Flash_Update_Index = 0;
uint32_t External_Flash_Address = 0;
uint32_t External_Flash_Start_Address = 0;

static void blink_led(int count)
{
    for (int i = 0; i < count; i++) {
        nrf_gpio_pin_set(0);
        nrf_delay_us(250000);
        nrf_gpio_pin_clear(0);
        nrf_delay_us(250000);
    }
    nrf_delay_us(500000);
}

void tick_system_seconds(void)
{
    system_seconds++;
}

uint32_t get_system_seconds(void)
{
    return system_seconds;
}

uint32_t system_millis(void)
{
    return (( (((uint64_t)RTC_OVERFLOW_COUNT << 24) | (uint64_t)NRF_RTC1->COUNTER) * 1.0 ) / (APP_TIMER_CLOCK_FREQ * 1.0)) * 1000.0;
}

uint32_t system_micros(void)
{
    return (( (((uint64_t)RTC_OVERFLOW_COUNT << 24) | (uint64_t)NRF_RTC1->COUNTER) * 1.0 ) / (APP_TIMER_CLOCK_FREQ * 1.0)) * 1000000.0;
}

/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    DEBUG("Hit an app error of code %d", error_code);
    DEBUG("Error happened on line number %d in file %s", line_num, p_file_name);
    LED_SetRGBColor(RGB_COLOR_RED);

    nrf_drv_wdt_channel_feed(m_channel_id);
    //this will stop all timers, so we need to feed the WDT here
    for (int i=0; i<APP_TIMER_MAX_TIMERS; i++) app_timer_stop(i);

    nrf_gpio_pin_set(0);
    for (int count = 0; count < 2; count++) {
        //SOS Call
        for (int i = 0; i < 3; i++) {
            LED_On(LED_RGB);
            nrf_delay_ms(100);
            LED_Off(LED_RGB);
            nrf_delay_ms(100);
            nrf_drv_wdt_channel_feed(m_channel_id);
        }
        nrf_delay_ms(250);
        for (int i = 0; i < 3; i++) {
            LED_On(LED_RGB);
            nrf_delay_ms(250);
            LED_Off(LED_RGB);
            nrf_delay_ms(250);
            nrf_drv_wdt_channel_feed(m_channel_id);
        }
        nrf_delay_ms(250);
        for (int i = 0; i < 3; i++) {
            LED_On(LED_RGB);
            nrf_delay_ms(100);
            LED_Off(LED_RGB);
            nrf_delay_ms(100);
            nrf_drv_wdt_channel_feed(m_channel_id);
        }
        nrf_delay_ms(1000);
        for (int i = 0; i < error_code; i++) {
            LED_On(LED_RGB);
            nrf_delay_ms(250);
            LED_Off(LED_RGB);
            nrf_delay_ms(250);
            nrf_drv_wdt_channel_feed(m_channel_id);
        }
        nrf_delay_ms(2000);
        nrf_drv_wdt_channel_feed(m_channel_id);
    }
    
    
    // On assert, the system can only recover with a reset.
    NVIC_SystemReset();
}

uint32_t OTA_FlashAddress()
{
    return FLASH_FW_ADDRESS;
}

#define FLASH_MAX_SIZE          (int32_t)(FLASH_STORAGE_ADDRESS - FLASH_FW_ADDRESS)

uint32_t OTA_FlashLength()
{
    return FLASH_MAX_SIZE;
}

uint16_t FLASH_GetDeviceInt(void)
{
    uint8_t byte1 = sFLASH_ReadSingleByte(FLASH_DEVICE_INT);
    uint8_t byte2 = sFLASH_ReadSingleByte(FLASH_DEVICE_INT+0x01);
    
    return (byte1 << 8)  |  byte2;
}

void FLASH_Begin(uint32_t sFLASH_Address, uint32_t fileSize)
{
    //OTA_FLASHED_Status_SysFlag = 0x0000;
    //Save_SystemFlags();
    for (int i=1; i<APP_TIMER_MAX_TIMERS; i++) app_timer_stop(i);

#if PLATFORM_ID==269
    disconnect_all_peripherals();
#endif
    isCloudUpdating = true;

    Flash_Update_Index = 0;
    External_Flash_Start_Address = sFLASH_Address;
    External_Flash_Address = External_Flash_Start_Address;

    /* Define the number of External Flash pages to be erased */
    NbrOfPage = FLASH_PagesMask(fileSize);

    /* Erase the SPI Flash pages */
    for (uint32_t EraseCounter = 0; (EraseCounter < NbrOfPage); EraseCounter++)
    {
        sFLASH_EraseSector(External_Flash_Start_Address + (sFLASH_PAGESIZE * EraseCounter));
    }
}

uint32_t FLASH_PagesMask(uint32_t fileSize)
{
    //Calculate the number of flash pages that needs to be erased
    uint32_t numPages = 0x0;

    if ((fileSize % sFLASH_PAGESIZE) != 0)
    {
        numPages = (fileSize / sFLASH_PAGESIZE) + 1;
    }
    else
    {
        numPages = fileSize / sFLASH_PAGESIZE;
    }

    return numPages;
}

uint16_t FLASH_Update(const uint8_t *pBuffer, uint32_t address, uint32_t bufferSize)
{
    const uint8_t *writeBuffer = pBuffer;
    uint8_t readBuffer[bufferSize];

    /* Write Data Buffer to SPI Flash memory */
    sFLASH_WriteBuffer(writeBuffer, address, bufferSize);

    /* Read Data Buffer from SPI Flash memory */
    sFLASH_ReadBuffer(readBuffer, address, bufferSize);

    /* Is the Data Buffer successfully programmed to SPI Flash memory */
    if (0 == memcmp(writeBuffer, readBuffer, bufferSize))
    {
        External_Flash_Address += bufferSize;
        Flash_Update_Index += 1;
    }
    else
    {
        /* Erase the problematic SPI Flash pages and back off the chunk index */
        External_Flash_Address = ((uint32_t)(External_Flash_Address / sFLASH_PAGESIZE)) * sFLASH_PAGESIZE;
        sFLASH_EraseSector(External_Flash_Address);
        Flash_Update_Index = (uint16_t)((External_Flash_Address - External_Flash_Start_Address) / bufferSize);
    }
    return Flash_Update_Index;
}

void FLASH_End(void)
{
    const module_info_t* module_info = FLASH_ModuleInfo(FLASH_SERIAL, FLASH_FW_ADDRESS);
    
    if (module_info != NULL) {
        uint32_t fw_len = (uint32_t)(External_Flash_Address - External_Flash_Start_Address);
        if (module_info->module_function==MODULE_FUNCTION_BOOTLOADER) {
            FLASH_CopyFW(FLASH_FW_ADDRESS, fw_len, false, true);
        } else {
            //erase the sector for now, so we can store the FW size and set the flag
            sFLASH_EraseSector(FLASH_FW_STATUS);

            uint8_t length1 = (uint8_t)((fw_len & 0xFF0000) >> 16);
            uint8_t length2 = (uint8_t)((fw_len & 0xFF00) >> 8);
            uint8_t length3 = (uint8_t)(fw_len & 0xFF);


            //TO DO: Set a flag in external flash to notify bootloader there is a FW update
            sFLASH_WriteSingleByte(FLASH_FW_STATUS, 0x01);
            sFLASH_WriteSingleByte(FLASH_FW_LENGTH1, length1);
            sFLASH_WriteSingleByte(FLASH_FW_LENGTH2, length2);
            sFLASH_WriteSingleByte(FLASH_FW_LENGTH3, length3);
        }
        //reboot
        NVIC_SystemReset();
    }
}

void system_init()
{
    system_connection_interval = 0;
}

//HW Init Functions
void leds_init(void)
{
    nrf_gpio_cfg_output(RGB_LED_PIN_RED);
    nrf_gpio_cfg_output(RGB_LED_PIN_GREEN);
    nrf_gpio_cfg_output(RGB_LED_PIN_BLUE);
    
    nrf_gpio_pin_set(RGB_LED_PIN_RED);
    nrf_gpio_pin_set(RGB_LED_PIN_GREEN);
    nrf_gpio_pin_set(RGB_LED_PIN_BLUE);

    nrf_gpio_cfg_output(0);
    nrf_gpio_pin_clear(0);
}
void timers_init(void)
{
      uint32_t err_code;
    // Initialize timer module, making it NOT use the scheduler
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, false);
    err_code = app_timer_create(&millis_timer, APP_TIMER_MODE_REPEATED, millis_timer_timeout);
    APP_ERROR_CHECK(err_code);
    
    //start a Timer for uSec resolution
    //TO DO - Do we want this? It keeps the HFCLK running which crushes battery life. Current implentation uses LFCLK
//    micros_timer.p_reg = NRF_TIMER1;
//    micros_timer.irq = TIMER1_IRQn;
//    micros_timer.instance_id = TIMER1_INSTANCE_INDEX;
//    
//    err_code = nrf_drv_timer_init(&micros_timer, NULL, micros_timer_timeout);
//    APP_ERROR_CHECK(err_code);
//    
//    uint32_t time_ticks =  nrf_drv_timer_us_to_ticks(&micros_timer, 100);
//    DEBUG("System Ticks per millisecond: %d", time_ticks);
//
//    nrf_drv_timer_extended_compare(
//                                   &micros_timer, NRF_TIMER_CC_CHANNEL1, time_ticks, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);
//    nrf_drv_timer_enable(&micros_timer);
}
void gpiote_init(void)
{
    //Commented out when moved to SDK 9.0, which moces away from app_gpiote to nrf_drv_gpiote
    //Do we need to do anything anymore?
//    APP_GPIOTE_INIT(APP_GPIOTE_MAX_USERS);
}
void buttons_init(void)
{
    //When upgrading to SDK9.0, we disabled APP_GPIOTE, on which this depends. Not sure we need it, so removing it for now.
//    static app_button_cfg_t buttons[] =
//    {
//        {BOARD_BUTTON, false, BUTTON_PULL, button_event_handler}
//    };
//    
//    APP_BUTTON_INIT(buttons, sizeof(buttons) / sizeof(buttons[0]), BUTTON_DETECTION_DELAY, true);
}

void disable_bootloader_flash(void)
{
    // issue #35: starting with v2.0.50 we swapped which TWI/SPI instance the user app uses from 1 to 0. however, the bootloader
    // still uses the original (1) instance, meaning that when the system firmware starts, the wrong SPI instance is enabled. so,
    // we need to disable the incorrect version before we go any further
    spi_master_disable(SPI1);
}

void external_flash_init(void)
{
    sFLASH_Init();
}

/**@brief Function for the Event Scheduler initialization.
 */
void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

void wdt_init(void)
{
    //Configure WDT.
    nrf_drv_wdt_config_t config = NRF_DRV_WDT_DEAFULT_CONFIG;
    uint32_t err_code = nrf_drv_wdt_init(&config, wdt_event_handler);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_wdt_channel_alloc(&m_channel_id);
    APP_ERROR_CHECK(err_code);
    nrf_drv_wdt_enable();
}

void wtd_feed(void)
{
    nrf_drv_wdt_channel_feed(m_channel_id);
}

/**@brief Function for the Device Manager initialization.
 */
void device_manager_init(void)
{
    uint32_t                err_code;
    dm_init_param_t         init_data;
    dm_application_param_t  register_param;
    bool                	bonds_delete = false;
    
    // Initialize peer device handle.
    err_code = dm_handle_initialize(&m_bonded_peer_handle);
    APP_ERROR_CHECK(err_code);
    //blink(1);
    // Initialize persistent storage module.
    err_code = pstorage_init();
    APP_ERROR_CHECK(err_code);
    //blink(2);
    // Clear all bonded centrals if the "delete all bonds" button is pushed.
    //SO STUPID! Youmust hand the index of the button from the Init call data structure, not just the pin number
    //It is 8 by the way
//    err_code = app_button_is_pushed(8, &bonds_delete);
//    APP_ERROR_CHECK(err_code);
    //    blink(3);
    init_data.clear_persistent_data = bonds_delete;

    err_code = dm_init(&init_data);
    APP_ERROR_CHECK(err_code);
    //blink(4);
    memset(&register_param.sec_param, 0, sizeof(ble_gap_sec_params_t));
    
    register_param.sec_param.bond         = SEC_PARAM_BOND;
    register_param.sec_param.mitm         = SEC_PARAM_MITM;
    register_param.sec_param.io_caps      = SEC_PARAM_IO_CAPABILITIES;
    register_param.sec_param.oob          = SEC_PARAM_OOB;
    register_param.sec_param.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
    register_param.sec_param.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
    register_param.evt_handler            = device_manager_evt_handler;

    //platofrm specific settings
#if PLATFORM_ID==103
    //bluz
    register_param.service_type           = DM_PROTOCOL_CNTXT_GATT_SRVR_ID;
#endif
#if PLATFORM_ID==269
    //bluz-gw
    register_param.service_type           = DM_PROTOCOL_CNTXT_GATT_CLI_ID;
#endif
    
    err_code = dm_register(&m_app_handle, &register_param);
    //blink(5);
    APP_ERROR_CHECK(err_code);
}

char* DEVICE_NAME = "Bluz DK";
void set_advertised_name(char* name)
{
    DEVICE_NAME = name;

    //if we are already advertising, reset to the new name
    if (state == BLE_ADVERTISING) {
        advertising_stop();
        gap_params_init();
        advertising_start();
    }
}

void start_ibeacon_advertising(uint16_t major, uint16_t minor, uint8_t *UUID)
{
    //if we are already advertising, reset to the new name
    if (state == BLE_ADVERTISING) {
        advertising_stop();
    }
    advertising_init_beacon(major, minor, UUID);
    advertising_start();
}

void start_eddystone_url_advertising(char *url)
{
    //if we are already advertising, reset to the new name
    if (state == BLE_ADVERTISING) {
        advertising_stop();
    }
    advertising_init_eddystone(url);
    advertising_start();
}

/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;
    
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);
    
    memset(&gap_conn_params, 0, sizeof(gap_conn_params));
    
    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;
    
    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the Connection Parameters module.
 */
void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;
    
    memset(&cp_init, 0, sizeof(cp_init));
    
    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = NULL;
    cp_init.error_handler                  = conn_params_error_handler;
    
    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

uint32_t timers_start(void)
{
    return app_timer_start(millis_timer, TIME_KEPPER_INTERVAL, NULL);
}

uint32_t timers_stop(void)
{
    return app_timer_stop(millis_timer);
}

int register_radio_callback(void (*radio_callback)(bool radio_active))
{
    return ble_radio_notification_init(NRF_APP_PRIORITY_LOW,NRF_RADIO_NOTIFICATION_DISTANCE_800US,radio_callback);
}

void register_data_callback(void (*data_callback)(uint8_t *data, uint16_t length))
{
    customDataServiceRegisterCallback(data_callback);
}

void register_event_callback(void (*event_callback)(uint8_t event, uint8_t *data, uint16_t length))
{
    infoDataServiceRegisterCallback(event_callback);
}

void send_data(uint8_t *data, uint16_t length)
{
    customDataServiceSendData(data, length);
}

int transmit_power = 0;
void setTxPower(int p)
{
    transmit_power = p;
    sd_ble_gap_tx_power_set(transmit_power);
}

void setConnParameters(int minimum, int maximum)
{
    ble_disconnect();
    int err_code;
    ble_gap_conn_params_t   gap_conn_params;
    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MSEC_TO_UNITS(minimum, UNIT_1_25_MS);
    gap_conn_params.max_conn_interval = MSEC_TO_UNITS(maximum, UNIT_1_25_MS);
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
void ble_stack_init(void)
{
    //Need a new init procedure for SD7
    uint32_t err_code;

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, false);

    // Enable BLE stack

    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));
    ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
    err_code = sd_ble_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing services that will be used by the application.
 */
void services_init(void)
{
    uint32_t err_code;
    scs_init_t init;
    
    init.data_write_handler = data_write_handler;
    
    err_code = scs_init(&m_scs, &init);
    APP_ERROR_CHECK(err_code);
}

ble_gap_adv_params_t m_adv_params;
void advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;

    ble_uuid_t adv_uuids[] = {{BLE_SCS_UUID_SERVICE, m_scs.uuid_type}};

    // Build and set advertising data
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance      = true;
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = adv_uuids;

    err_code = ble_advdata_set(&advdata, &scanrsp);
    APP_ERROR_CHECK(err_code);

    //set the advertising parameters
    memset(&m_adv_params, 0, sizeof(m_adv_params));

    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    m_adv_params.p_peer_addr = NULL;                             // Undirected advertisement.
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = APP_ADV_INTERVAL;
    m_adv_params.timeout    = APP_ADV_NO_TIMEOUT;
}

uint8_t APP_BEACON_UUID[16] = {0};
void advertising_init_beacon(uint16_t major, uint16_t minor, uint8_t *UUID)
{
    uint8_t m_beacon_info[APP_BEACON_INFO_LENGTH];

    m_beacon_info[0] = (uint8_t)APP_DEVICE_TYPE; // Manufacturer specific information. Specifies the device type in this implementation.
    m_beacon_info[1] = (uint8_t)APP_ADV_DATA_LENGTH; // Manufacturer specific information. Specifies the length of the manufacturer specific data in this implementation.
    memcpy(m_beacon_info + 2, UUID, 16);
    m_beacon_info[18] = (uint8_t)(major >> 8);
    m_beacon_info[19] = (uint8_t)(major);
    m_beacon_info[20] = (uint8_t)(minor >> 8);
    m_beacon_info[21] = (uint8_t)(minor);
    m_beacon_info[22] = APP_MEASURED_RSSI+transmit_power;

    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;
    uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    ble_advdata_manuf_data_t manuf_specific_data;

    manuf_specific_data.company_identifier = APP_COMPANY_IDENTIFIER;

    manuf_specific_data.data.p_data = (uint8_t *) m_beacon_info;
    manuf_specific_data.data.size   = APP_BEACON_INFO_LENGTH;

    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type             = BLE_ADVDATA_NO_NAME;
    advdata.flags                 = flags;
    advdata.p_manuf_specific_data = &manuf_specific_data;

    ble_uuid_t adv_uuids[] = {{BLE_SCS_UUID_SERVICE, m_scs.uuid_type}};

    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = adv_uuids;
    scanrsp.name_type = BLE_ADVDATA_FULL_NAME;

    err_code = ble_advdata_set(&advdata, &scanrsp);
    APP_ERROR_CHECK(err_code);

    //set the advertising parameters
    memset(&m_adv_params, 0, sizeof(m_adv_params));

    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    m_adv_params.p_peer_addr = NULL;                             // Undirected advertisement.
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = BEACON_ADV_INTERVAL;
    m_adv_params.timeout     = APP_ADV_NO_TIMEOUT;
}

void advertising_init_eddystone(char *url)
{
    uint32_t err_code;

    eddystone_beacon_payload packet = parse_url(url);

    //we will truncate the URL, otherwise we get an error as max BLE advertise size is 31 bytes
    if (packet.length > MAX_URL_LENGTH) {
        packet.length = MAX_URL_LENGTH;
    }

    int length = EDDYSTONE_BEACON_LENGTH+packet.length;

    uint8_t eddystone_url_data[length];
    eddystone_url_data[0] = EDDYSTONE_FRAME_TYPE;
    eddystone_url_data[1] = EDDYSTONE_TX_POWER;
    memcpy(eddystone_url_data+2, packet.data, packet.length);

    ble_advdata_t advdata;
    uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    ble_uuid_t    adv_uuids[] = {{APP_EDDYSTONE_UUID, BLE_UUID_TYPE_BLE}};

    uint8_array_t eddystone_data_array;                             // Array for Service Data structure.
    /** @snippet [Eddystone data array] */
    eddystone_data_array.p_data = (uint8_t *) eddystone_url_data;   // Pointer to the data to advertise.
    eddystone_data_array.size = sizeof(eddystone_url_data);         // Size of the data to advertise.
    /** @snippet [Eddystone data array] */

    ble_advdata_service_data_t service_data;                        // Structure to hold Service Data.
    service_data.service_uuid = APP_EDDYSTONE_UUID;                 // Eddystone UUID to allow discoverability on iOS devices.
    service_data.data = eddystone_data_array;                       // Array for service advertisement data.

    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_NO_NAME;
    advdata.flags                   = flags;
    advdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    advdata.uuids_complete.p_uuids  = adv_uuids;
    advdata.p_service_data_array    = &service_data;                // Pointer to Service Data structure.
    advdata.service_data_count      = 1;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);

    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));

    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    m_adv_params.p_peer_addr = NULL;                             // Undirected advertisement.
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = BEACON_ADV_INTERVAL;
    m_adv_params.timeout     = APP_ADV_NO_TIMEOUT;
}

//Startup Functions

/**@brief Function for starting advertising.
 */
void advertising_start(void)
{
    uint32_t             err_code;
    
    if (state != BLE_ADVERTISING) {
        err_code = sd_ble_gap_adv_start(&m_adv_params);
        APP_ERROR_CHECK(err_code);
        state = BLE_ADVERTISING;
    }
}

/**@brief Function for stopping advertising.
 */
void advertising_stop(void)
{
    uint32_t             err_code;
    if (state == BLE_ADVERTISING) {
        err_code = sd_ble_gap_adv_stop();
        APP_ERROR_CHECK(err_code);
        state = BLE_OFF;
    }
}

/**@brief Function for disconnectiong from ble.
 */
void ble_disconnect(void)
{
    uint32_t             err_code;
    if (state == BLE_CONNECTED) {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        APP_ERROR_CHECK(err_code);
    }
}

void set_cloud_connection_state(bool connected)
{
    isCloudConnected = connected;
}

/**@brief Function for the Power manager.
 */
void power_manage(void)
{
//    uint32_t err_code = timers_stop();
//    APP_ERROR_CHECK(err_code);
//    
//    sd_nvic_ClearPendingIRQ(RTC1_IRQn);
//    nrf_drv_timer_disable(&micros_timer);
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
//    nrf_drv_timer_enable(&micros_timer);
//    err_code = timers_start();
//    APP_ERROR_CHECK(err_code);
}

void shutdown(void)
{
    nrf_drv_wdt_channel_feed(m_channel_id);
    //this will stop all timers, so we need to feed the WDT here
    for (int i=0; i<APP_TIMER_MAX_TIMERS; i++) app_timer_stop(i);
    LED_Off(LED_RGB);
    sd_power_system_off();
}

void FLASH_WriteProtection_Enable(uint32_t FLASH_Sectors)
{

}

void FLASH_WriteProtection_Disable(uint32_t FLASH_Sectors)
{
    
}

static uint32_t crc32_tab[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

uint32_t Compute_CRC32(uint32_t crc, const uint8_t* buf, size_t size)
{
    const uint8_t *p = buf;
    crc = crc ^ ~0U;
    
    while (size--)
        crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
    
    return crc ^ ~0U;
}

