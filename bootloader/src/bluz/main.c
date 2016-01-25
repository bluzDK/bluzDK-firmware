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
#undef STATIC_ASSERT
#include "flash.h"
#include "module_info.h"
#include "app_uart.h"
#include "rgbled_hal.h"

#define IS_SRVC_CHANGED_CHARACT_PRESENT 1                                                       /**< Include the service_changed characteristic. For DFU this should normally be the case. */

#define BOOTLOADER_BUTTON               BOARD_BUTTON                                            /**< Button used to enter SW update mode. */
#define UPDATE_IN_PROGRESS_LED          RGB_LED_PIN_BLUE                                        /**< Led used to indicate that DFU is active. */

#define APP_TIMER_PRESCALER             0                                                       /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS            3                                                       /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                                       /**< Size of timer operation queues. */

#define SCHED_MAX_EVENT_DATA_SIZE       MAX(APP_TIMER_SCHED_EVT_SIZE, 0)                        /**< Maximum size of scheduler events. */

#define SCHED_QUEUE_SIZE                20                                                      /**< Maximum number of events in the scheduler queue. */



void uart_put(char *str) {
    uint_fast8_t i  = 0;
    uint8_t      ch = str[i++];
    while (ch != '\0')
    {
        while (app_uart_put(ch) == NRF_ERROR_NO_MEM) { }
//        app_uart_put(ch);
        ch = str[i++];
    }
}

void uart_error_handle(app_uart_evt_t * p_event);

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
        Set_RGB_LED_Values(0,0,255);
        nrf_delay_ms(200);
        Set_RGB_LED_Values(0,0,0);
        nrf_delay_ms(200);
    }
    nrf_delay_ms(450);
}

void copyFW(uint32_t flashFWLocation, uint32_t fw_len, bool wipeUserApp)
{
    uint32_t         err_code;
    
    Set_RGB_LED_Values(0,0,255);
    
    //let's init the pstorage
    pstorage_handle_t m_storage_handle_app;
    pstorage_module_param_t storage_module_param = {.cb = pstorage_callback_handler};
    
    if (wipeUserApp) {
        //hack for now...
        fw_len += 0x4000;
    }
    
    storage_module_param.block_size = 0x100;
    storage_module_param.block_count = fw_len / 256;
    
    pstorage_init();
    err_code = pstorage_raw_register(&storage_module_param, &m_storage_handle_app);
    APP_ERROR_CHECK(err_code);
    
    const module_info_t* modinfo = FLASH_ModuleInfo(FLASH_SERIAL, flashFWLocation);
    m_storage_handle_app.block_id  = (uint32_t)modinfo->module_start_address;
    
    if (!FLASH_isUserModuleInfoValid(FLASH_SERIAL, flashFWLocation, 0x00)) {
        uart_put("BAD MODULE. NOT GOING TO FLASH\n");
        bootloader_app_start(DFU_BANK_0_REGION_START);
    }
    uart_put("GOOD MODULE. FLASHING\n");
    
    
    
    uart_put("GOOD MODULE. FLASHING\n");
    
    uart_put("Flashing over ");
    char str0[80];
    sprintf(str0, "%d", (int)fw_len);
    uart_put(str0);
    uart_put(" bytes\n");
    
    uart_put("To address ");
    char str1[80];
    sprintf(str1, "%d", (int)m_storage_handle_app.block_id);
    uart_put(str1);
    uart_put("\n");
    
    uart_put("For platform ");
    char str2[80];
    sprintf(str2, "%d", (int)modinfo->platform_id);
    uart_put(str2);
    uart_put("\n");
    
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
    uint32_t addr = flashFWLocation;
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
    Set_RGB_LED_Values(0,0,0);
}

/**@brief Function for bootloader main entry.
 */
int main(void)
{
    
//    bool     dfu_start = false;
    bool     app_reset = (NRF_POWER->GPREGRET == BOOTLOADER_DFU_START);
    
    uint32_t         err_code;
    const app_uart_comm_params_t comm_params =
    {
        12,
        8,
        20,
        11,
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
        UART_BAUDRATE_BAUDRATE_Baud38400
    };

    APP_UART_INIT(&comm_params,
         uart_error_handle,
         APP_IRQ_PRIORITY_LOW,
         err_code);
    APP_ERROR_CHECK(err_code);
    
    uart_put("STARTING!\n");
    

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
    scheduler_init();

    //init external flash then check if update is ready
    sFLASH_Init();
    
    
    //copy device int to external flash
    uint8_t buf[2];
    memcpy(buf, (const void *)0x3F000, 2);
    sFLASH_EraseSector(FLASH_DEVICE_INT);
    sFLASH_WriteBuffer(buf, FLASH_DEVICE_INT, 2);
    blink(1);
    
    int len = 0;
    //copy private key to external flash
    len = 612;
    uint8_t privateKeyBuf[len];
    memcpy(privateKeyBuf, (const void *)0x3F400, len);
    sFLASH_EraseSector(FLASH_PRIVATE_KEY);
    sFLASH_WriteBuffer(privateKeyBuf, FLASH_PRIVATE_KEY, len);
    blink(1);
    
    //copy public key to external flash
    len = 296;
    uint8_t pubKeyBuf[len];
    memcpy(pubKeyBuf, (const void *)0x3F800, len);
    sFLASH_EraseSector(FLASH_PUBLIC_KEY);
    sFLASH_WriteBuffer(pubKeyBuf, FLASH_PUBLIC_KEY, len);
    blink(1);
    
    //copy system part to external flash
    //...but first, prepare SPI flash by erasing everything
    for (unsigned int i = FACTORY_RESET_FW_ADDRESS; i < FLASH_FW_ADDRESS; i+=0x1000) {
        sFLASH_EraseSector(FACTORY_RESET_FW_ADDRESS+i);
    }
    len = 1024;
    uint8_t fwBuf[len];
    for (unsigned int i = 0x00; i < FACTORY_RESET_FW_SIZE; i+=len) {
        memcpy(fwBuf, (const void *)(0x00018000+i), len);
        sFLASH_WriteBuffer(fwBuf, FACTORY_RESET_FW_ADDRESS+i, len);
    }
    blink(1);
    uart_put("Done\n");
    
    //forever rainbows!
    Set_RGB_LED_Values(255,0,0);
    while (1) {
        Set_RGB_LED_Values(255,0,0);
        nrf_delay_ms(250);
        Set_RGB_LED_Values(0,255,0);
        nrf_delay_ms(250);
        Set_RGB_LED_Values(0,0,255);
        nrf_delay_ms(250);
        Set_RGB_LED_Values(255,255,0);
        nrf_delay_ms(250);
        Set_RGB_LED_Values(255,0,255);
        nrf_delay_ms(250);
        Set_RGB_LED_Values(255,255,255);
        nrf_delay_ms(250);
    }       
}
