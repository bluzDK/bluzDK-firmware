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

#include "nrf51_callbacks.h"
#include "data_management_layer.h"
#include "core_hal.h"
#include "ble_conn_params.h"
#include "pstorage.h"

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
    switch (pin_no)
    {
        case BOARD_BUTTON:
            //BOARD BUTTON PRESSED, WHAT DO WE DO?
            break;
            
        default:
            //APP_ERROR_HANDLER(pin_no);
            //UNKNOWN BUTTON PRESSED? HOW COULD THIS BE?!?
            break;
    }
}

void millis_timer_timeout(void * p_context)
{
    //TO DO
    //This will get called every ms from the RTC, keep track of time
//    system_milliseconds++;
    HAL_SysTick_Handler();
}

void micros_timer_timeout(nrf_timer_event_t event_type, void* p_context)
{
    //This will get called every us from the timer
    switch(event_type)
    {
        case NRF_TIMER_EVENT_COMPARE1:
            system_microseconds++;
            if (system_microseconds > 20000) {
                nrf_gpio_pin_set(30);
            }
            break;
        case NRF_TIMER_EVENT_COMPARE3:
        case NRF_TIMER_EVENT_COMPARE2:
        case NRF_TIMER_EVENT_COMPARE0:
            break;
    }
}

/**
 * @brief WDT events handler.
 */
void wdt_event_handler(void)
{
    //NOTE: The max amount of time we can spend in WDT interrupt is two cycles of 32768[Hz] clock - after that, reset occurs
}

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
void on_ble_evt(ble_evt_t * p_ble_evt)
{
//    uint32_t                         err_code;
//    static ble_gap_evt_auth_status_t m_auth_status;
//    ble_gap_enc_info_t *             p_enc_info;
    ble_gap_addr_t* address = 0;
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            
            sd_ble_gap_address_get(address);
            
            system_connection_interval = p_ble_evt->evt.gap_evt.params.conn_param_update.conn_params.max_conn_interval;
            state = BLE_CONNECTED;
            break;
            
        case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            advertising_start();
            state = BLE_ADVERTISING;
            break;
            
//        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
//            err_code = sd_ble_gap_sec_params_reply(m_conn_handle,
//                                                   BLE_GAP_SEC_STATUS_SUCCESS,
//                                                   &m_sec_params, NULL);
//            APP_ERROR_CHECK(err_code);
//            break;
//            
//        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
//            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
//            APP_ERROR_CHECK(err_code);
//            break;
//            
//        case BLE_GAP_EVT_AUTH_STATUS:
//            m_auth_status = p_ble_evt->evt.gap_evt.params.auth_status;
//            break;
//            
//        case BLE_GAP_EVT_SEC_INFO_REQUEST:
//            p_enc_info = &m_auth_status.periph_keys.enc_info;
//            if (p_enc_info->div == p_ble_evt->evt.gap_evt.params.sec_info_request.div)
//            {
//                err_code = sd_ble_gap_sec_info_reply(m_conn_handle, p_enc_info, NULL);
//                APP_ERROR_CHECK(err_code);
//            }
//            else
//            {
//                // No keys found for this device
//                err_code = sd_ble_gap_sec_info_reply(m_conn_handle, NULL, NULL);
//                APP_ERROR_CHECK(err_code);
//            }
//            break;
            
        case BLE_GAP_EVT_TIMEOUT:
            if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISING)
            {
                
                // Configure buttons with sense level low as wakeup source.
                nrf_gpio_cfg_sense_input(BOARD_BUTTON,
                                         BUTTON_PULL,
                                         NRF_GPIO_PIN_SENSE_LOW);
                
                // Go to system-off mode (this function will not return; wakeup will cause a reset)
                advertising_stop();
            }
            break;
            
        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
            system_connection_interval = p_ble_evt->evt.gap_evt.params.conn_param_update.conn_params.max_conn_interval;
            break;
        default:
            // No implementation needed.
            break;
    }
}

/**@brief Function for dispatching a BLE stack event to all modules with a BLE stack event handler.
 *
 * @details This function is called from the scheduler in the main loop after a BLE stack
 *          event has been received.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    on_ble_evt(p_ble_evt);
    ble_conn_params_on_ble_evt(p_ble_evt);
    scs_on_ble_evt(&m_scs, p_ble_evt);
}

/**@brief Function for dispatching a system event to interested modules.
 *
 * @details This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * @param[in]   sys_evt   System stack event.
 */
void sys_evt_dispatch(uint32_t sys_evt)
{
    //TO DO: When pstorage is turned back on, this is necessary
    pstorage_sys_event_handler(sys_evt);
}

void data_write_handler(scs_t * p_lbs, uint8_t *data, uint16_t length)
{

    uint16_t serviceID = 0x00 | data[0];
    dataManagementFeedData(serviceID, length-1, data+1);
//    uint16_t serviceID = 0x01;
//    dataManagementFeedData(serviceID, length, data);
}

/**@brief Function for handling the Device Manager events.
 *
 * @param[in]   p_evt   Data associated to the device manager event.
 */
uint32_t device_manager_evt_handler(dm_handle_t const    * p_handle,
                                           dm_event_t const     * p_event,
                                           ret_code_t           event_result)
{
    APP_ERROR_CHECK(event_result);
    
    switch(p_event->event_id)
    {
        case DM_EVT_DEVICE_CONTEXT_LOADED: // Fall through.
        case DM_EVT_SECURITY_SETUP_COMPLETE:
            m_bonded_peer_handle = (*p_handle);
            break;
    }
    
    return NRF_SUCCESS;
}