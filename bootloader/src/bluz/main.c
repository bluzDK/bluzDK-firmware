/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/**@file
 *
 * @defgroup ble_sdk_app_bootloader_main main.c
 * @{
 * @ingroup dfu_bootloader_api
 * @brief Bootloader project main file.
 *
 * -# Receive start data packet. 
 * -# Based on start packet, prepare NVM area to store received data. 
 * -# Receive data packet. 
 * -# Validate data packet.
 * -# Write Data packet to NVM.
 * -# If not finished - Wait for next packet.
 * -# Receive stop data packet.
 * -# Activate Image, boot application.
 *
 */
#include "dfu_transport.h"
#include "bootloader.h"
#include "bootloader_util.h"
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_soc.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "nrf51_bitfields.h"
#include "ble.h"
#include "nrf51.h"
#include "ble_hci.h"
#include "app_scheduler.h"
#include "app_timer_appsh.h"
#include "nrf_error.h"
//#include "bsp.h"
#include "softdevice_handler_appsh.h"
#include "pstorage_platform.h"
#include "nrf_mbr.h"
#include "sst25vf_spi.h"
#include "hw_layout.h"
#include "nrf_delay.h"
#include "pstorage.h"

#define IS_SRVC_CHANGED_CHARACT_PRESENT 1                                                       /**< Include the service_changed characteristic. For DFU this should normally be the case. */

#define BOOTLOADER_BUTTON               BOARD_BUTTON                                            /**< Button used to enter SW update mode. */
#define UPDATE_IN_PROGRESS_LED          RGB_LED_PIN_BLUE                                        /**< Led used to indicate that DFU is active. */

#define APP_TIMER_PRESCALER             0                                                       /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS            3                                                       /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                                       /**< Size of timer operation queues. */

#define SCHED_MAX_EVENT_DATA_SIZE       MAX(APP_TIMER_SCHED_EVT_SIZE, 0)                        /**< Maximum size of scheduler events. */

#define SCHED_QUEUE_SIZE                20                                                      /**< Maximum number of events in the scheduler queue. */


/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze 
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] file_name   File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}


/**@brief Function for initialization of LEDs.
 */
static void leds_init(void)
{
    nrf_gpio_cfg_output(RGB_LED_PIN_RED);
    nrf_gpio_cfg_output(RGB_LED_PIN_GREEN);
    nrf_gpio_cfg_output(RGB_LED_PIN_BLUE);
    
    nrf_gpio_pin_set(RGB_LED_PIN_RED);
    nrf_gpio_pin_set(RGB_LED_PIN_GREEN);
    nrf_gpio_pin_set(RGB_LED_PIN_BLUE);
}


/**@brief Function for initializing the timer handler module (app_timer).
 */
static void timers_init(void)
{
    // Initialize timer module, making it use the scheduler.
    APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, true);
}


/**@brief Function for initializing the button module.
 */
static void buttons_init(void)
{
    nrf_gpio_cfg_sense_input(BOOTLOADER_BUTTON,
                             BUTTON_PULL, 
                             NRF_GPIO_PIN_SENSE_LOW);

}


/**@brief Function for dispatching a BLE stack event to all modules with a BLE stack event handler.
 *
 * @details This function is called from the scheduler in the main loop after a BLE stack
 *          event has been received.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void sys_evt_dispatch(uint32_t event)
{
    pstorage_sys_event_handler(event);
}

/**@brief Function for handling callbacks from pstorage module.
 *
 * @details Handles pstorage results for clear and storage operation. For detailed description of
 *          the parameters provided with the callback, please refer to \ref pstorage_ntf_cb_t.
 */
bool stillWorking = false;
static void pstorage_callback_handler(pstorage_handle_t * p_handle,
                                      uint8_t             op_code,
                                      uint32_t            result,
                                      uint8_t           * p_data,
                                      uint32_t            data_len)
{
    switch (op_code)
    {
        case PSTORAGE_STORE_OP_CODE:
            
            break;
            
        case PSTORAGE_CLEAR_OP_CODE:
            
            if (result == NRF_SUCCESS)
            {
                stillWorking = false;
                return;
            }
            else
            {
                stillWorking = false;
                return;
            }
            break;
            
        default:
            break;
    }
    APP_ERROR_CHECK(result);
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 *
 * @param[in] init_softdevice  true if SoftDevice should be initialized. The SoftDevice must only 
 *                             be initialized if a chip reset has occured. Soft reset from 
 *                             application must not reinitialize the SoftDevice.
 */
static void ble_stack_init(bool init_softdevice)
{
    uint32_t         err_code;
    sd_mbr_command_t com = {SD_MBR_COMMAND_INIT_SD, };

    if (init_softdevice)
    {
        err_code = sd_mbr_command(&com);
        APP_ERROR_CHECK(err_code);
    }
    
    err_code = sd_softdevice_vector_table_base_set(BOOTLOADER_REGION_START);
    APP_ERROR_CHECK(err_code);
   
    SOFTDEVICE_HANDLER_APPSH_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, true);

    // Enable BLE stack 
    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));
    
    // Below code line is needed for s130. For s110 is inrrelevant - but executable
    // can run with both s130 and s110.
    ble_enable_params.gatts_enable_params.attr_tab_size   = BLE_GATTS_ATTR_TAB_SIZE_DEFAULT;

    ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
    err_code = sd_ble_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);
    
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for event scheduler initialization.
 */
static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

void blink(int times)
{
    for (int i = 0; i < times; i++) {
        nrf_gpio_pin_clear(RGB_LED_PIN_BLUE);
        nrf_delay_ms(100);
        nrf_gpio_pin_set(RGB_LED_PIN_BLUE);
        nrf_delay_ms(100);
    }
    nrf_delay_ms(450);
}


/**@brief Function for bootloader main entry.
 */
int main(void)
{
    uint32_t err_code;
//    bool     dfu_start = false;
    bool     app_reset = (NRF_POWER->GPREGRET == BOOTLOADER_DFU_START);

    if (app_reset)
    {
        NRF_POWER->GPREGRET = 0;
    }
    
    leds_init();

    // This check ensures that the defined fields in the bootloader corresponds with actual
    // setting in the nRF51 chip.
    APP_ERROR_CHECK_BOOL(*((uint32_t *)NRF_UICR_BOOT_START_ADDRESS) == BOOTLOADER_REGION_START);
    APP_ERROR_CHECK_BOOL(NRF_FICR->CODEPAGESIZE == CODE_PAGE_SIZE);

    // Initialize.
    timers_init();
    buttons_init();
    
    //For now, until you implement the full bootloader
    ble_stack_init(true);
    
    //init external flash then check if update is ready
    sFLASH_Init();
    uint8_t byte0 = sFLASH_ReadSingleByte(FLASH_FW_STATUS);
    if (byte0 == 0x01) {
        //we have an app waiting for us. let's first find out the length
        uint32_t fw_len = 0;
        uint8_t byte1 = sFLASH_ReadSingleByte(FLASH_FW_LENGTH1);
        uint8_t byte2 = sFLASH_ReadSingleByte(FLASH_FW_LENGTH2);
        uint8_t byte3 = sFLASH_ReadSingleByte(FLASH_FW_LENGTH3);
        fw_len = (byte1 << 16) | (byte2 << 8)  |  byte3;
        
        //let's init the pstorage
        pstorage_handle_t m_storage_handle_app;
        pstorage_module_param_t storage_module_param = {.cb = pstorage_callback_handler};
        
        storage_module_param.block_size = 0x100;
        storage_module_param.block_count = fw_len / 256;
        
        pstorage_init();
        err_code = pstorage_raw_register(&storage_module_param, &m_storage_handle_app);
        APP_ERROR_CHECK(err_code);
        
        m_storage_handle_app.block_id  = DFU_BANK_0_REGION_START;
        
        uint32_t    ops_count = 7;
        //now clear the nrf51 flash
        stillWorking = true;
        err_code = pstorage_raw_clear(&m_storage_handle_app, fw_len);
        do {
            app_sched_execute();
            pstorage_access_status_get(&ops_count);
        }
        while(ops_count != 0);
        
        app_sched_execute();
        
        APP_ERROR_CHECK(err_code);
        
        //now read from SPI Flash one page at a time and copy over to internal flash
        uint8_t buf[PSTORAGE_FLASH_PAGE_SIZE];
        uint32_t addr = FLASH_FW_ADDRESS;
        for (int i = 0; i < fw_len; i+=PSTORAGE_FLASH_PAGE_SIZE) {
            sFLASH_ReadBuffer(buf, addr, PSTORAGE_FLASH_PAGE_SIZE);
            err_code = pstorage_raw_store(&m_storage_handle_app,
                                          (uint8_t *)buf,
                                          PSTORAGE_FLASH_PAGE_SIZE,
                                          i);
            APP_ERROR_CHECK(err_code);
            
            do {
                app_sched_execute();
                pstorage_access_status_get(&ops_count);
            }
            while(ops_count != 0);
            
            
            addr+=PSTORAGE_FLASH_PAGE_SIZE;
        }
        sFLASH_EraseSector(FLASH_FW_STATUS);
        sFLASH_WriteSingleByte(FLASH_FW_STATUS, 0x00);
    }
    //TO DO: Temporary for now, just boot directly into the app.
    //Really, we should go on and see if they want to enter boot mode, then take FW and keys through DFU
    bootloader_app_start(DFU_BANK_0_REGION_START);

//    (void)bootloader_init();
//
//    if (bootloader_dfu_sd_in_progress())
//    {
//        nrf_gpio_pin_clear(UPDATE_IN_PROGRESS_LED);
//
//        err_code = bootloader_dfu_sd_update_continue();
//        APP_ERROR_CHECK(err_code);
//
//        ble_stack_init(!app_reset);
//        scheduler_init();
//
//        err_code = bootloader_dfu_sd_update_finalize();
//        APP_ERROR_CHECK(err_code);
//
//        nrf_gpio_pin_set(UPDATE_IN_PROGRESS_LED);
//    }
//    else
//    {
//        // If stack is present then continue initialization of bootloader.
//        ble_stack_init(!app_reset);
//        scheduler_init();
//    }
//
//    dfu_start  = app_reset;
//    dfu_start |= ((nrf_gpio_pin_read(BOOTLOADER_BUTTON) == 0) ? true: false);
//    
//    if (dfu_start || (!bootloader_app_is_valid(DFU_BANK_0_REGION_START)))
//    {
//        nrf_gpio_pin_clear(UPDATE_IN_PROGRESS_LED);
//
//        // Initiate an update of the firmware.
//        err_code = bootloader_dfu_start();
//        APP_ERROR_CHECK(err_code);
//
//        nrf_gpio_pin_set(UPDATE_IN_PROGRESS_LED);
//    }

    if (bootloader_app_is_valid(DFU_BANK_0_REGION_START) && !bootloader_dfu_sd_in_progress())
    {
        // Select a bank region to use as application region.
        // @note: Only applications running from DFU_BANK_0_REGION_START is supported.
        bootloader_app_start(DFU_BANK_0_REGION_START);
    }
    
    NVIC_SystemReset();
}
