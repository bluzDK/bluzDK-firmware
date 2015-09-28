#include <string.h>

#include "hw_config.h"
#include "sst25vf_spi.h"
#include "data_management_layer.h"
#include "socket_manager.h"
#include "debug.h"
#include "rgbled.h"

uint32_t NbrOfPage = 0;
uint16_t Flash_Update_Index = 0;
uint32_t External_Flash_Address = 0;
uint32_t External_Flash_Start_Address = 0;

uint32_t system_millis(void)
{
    return system_millseconds;
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
    for (int count = 0; count < 2; count++) {
        //SOS Call
        for (int i = 0; i < 3; i++) {
            LED_On(LED_RGB);
            nrf_delay_ms(100);
            LED_Off(LED_RGB);
            nrf_delay_ms(100);
        }
        nrf_delay_ms(250);
        for (int i = 0; i < 3; i++) {
            LED_On(LED_RGB);
            nrf_delay_ms(250);
            LED_Off(LED_RGB);
            nrf_delay_ms(250);
        }
        nrf_delay_ms(250);
        for (int i = 0; i < 3; i++) {
            LED_On(LED_RGB);
            nrf_delay_ms(100);
            LED_Off(LED_RGB);
            nrf_delay_ms(100);
        }
        nrf_delay_ms(1000);
        for (int i = 0; i < error_code; i++) {
            LED_On(LED_RGB);
            nrf_delay_ms(250);
            LED_Off(LED_RGB);
            nrf_delay_ms(250);
        }
        nrf_delay_ms(3000);
    }
    
    
    // On assert, the system can only recover with a reset.
    NVIC_SystemReset();
}

uint32_t OTA_FlashAddress()
{
    return FLASH_FW_ADDRESS;
}

#define FLASH_MAX_SIZE          (int32_t)(FLASH_LENGTH - FLASH_FW_ADDRESS)

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
	uint32_t fw_len = (uint32_t)(External_Flash_Address - External_Flash_Start_Address);

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
    
    //reboot
    NVIC_SystemReset();
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
}
void timers_init(void)
{
      uint32_t err_code;
    // Initialize timer module, making it use the scheduler
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, false);
    err_code = app_timer_create(&millis_timer, APP_TIMER_MODE_REPEATED, millis_timer_timeout);
    APP_ERROR_CHECK(err_code);
}
void gpiote_init(void)
{
    APP_GPIOTE_INIT(APP_GPIOTE_MAX_USERS);
}
void buttons_init(void)
{
    static app_button_cfg_t buttons[] =
    {
        {BOARD_BUTTON, false, BUTTON_PULL, button_event_handler}
    };
    
    APP_BUTTON_INIT(buttons, sizeof(buttons) / sizeof(buttons[0]), BUTTON_DETECTION_DELAY, true);
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

uint32_t timers_start(void)
{
    return app_timer_start(millis_timer, TIME_KEPPER_INTERVAL, NULL);
}

uint32_t timers_stop(void)
{
    return app_timer_stop(millis_timer);
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

void advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;
    uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;
    
    ble_uuid_t adv_uuids[] = {{BLE_SCS_UUID_SERVICE, m_scs.uuid_type}};
    
    // Build and set advertising data
    memset(&advdata, 0, sizeof(advdata));
    
    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance      = true;
    advdata.flags.size              = sizeof(flags);
    advdata.flags.p_data            = &flags;
    
    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = adv_uuids;
    
    err_code = ble_advdata_set(&advdata, &scanrsp);
    APP_ERROR_CHECK(err_code);
}

//Startup Functions

/**@brief Function for starting advertising.
 */
void advertising_start(void)
{
    uint32_t             err_code;
    ble_gap_adv_params_t adv_params;
    
    // Start advertising
    memset(&adv_params, 0, sizeof(adv_params));
    
    adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    adv_params.p_peer_addr = NULL;
    adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    adv_params.interval    = APP_ADV_INTERVAL;
    adv_params.timeout     = APP_ADV_TIMEOUT_IN_SECONDS;
    
    err_code = sd_ble_gap_adv_start(&adv_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for the Power manager.
 */
void power_manage(void)
{
//    uint32_t err_code = timers_stop();
//    APP_ERROR_CHECK(err_code);
//    
//    sd_nvic_ClearPendingIRQ(RTC1_IRQn);
    
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
    
//    err_code = timers_start();
//    APP_ERROR_CHECK(err_code);
}

