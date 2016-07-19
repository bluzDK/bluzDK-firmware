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

#include "nrf51_config.h"
#include "nrf51_callbacks.h"
#include "data_management_layer.h"
#include "core_hal.h"
#include "ble_conn_params.h"
#include "pstorage.h"
#include "hw_gateway_config.h"
#include "client_handling.h"
#include "ble.h"
#include "ble_srv_common.h"

void uart_error_handle(app_uart_evt_t * p_event)
{
//    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
//    {
//        APP_ERROR_HANDLER(p_event->data.error_communication);
//    }
//    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
//    {
//        APP_ERROR_HANDLER(p_event->data.error_code);
//    }
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
//    static ble_gap_evt_auth_status_t m_auth_status;
//    ble_gap_enc_info_t *             p_enc_info;
    
    switch (p_ble_evt->header.evt_id)
    {
#if PLATFORM_ID==103
        case BLE_GAP_EVT_CONNECTED:
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

            ble_gap_addr_t* address = 0;
            sd_ble_gap_address_get(address);
            
            system_connection_interval = p_ble_evt->evt.gap_evt.params.conn_param_update.conn_params.max_conn_interval;

            state = BLE_GAP_CONNECTED;
            break;

        case BLE_GATTS_EVT_WRITE:
            if (p_ble_evt->evt.gatts_evt.params.write.len == 2 && p_ble_evt->evt.gatts_evt.params.write.op == BLE_GATT_OP_WRITE_REQ
            && p_ble_evt->evt.gatts_evt.params.write.data[0] == BLE_GATT_HVX_NOTIFICATION) {
                //this is the notification we have been waiting for!
                state = BLE_CONNECTED;
            }
            break;
            
        case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            advertising_start();
            state = BLE_ADVERTISING;
            break;

        case BLE_GAP_EVT_TIMEOUT:
            //bluz
            if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISING)
            {

                // Configure buttons with sense level low as wakeup source.
                nrf_gpio_cfg_sense_input(BOARD_BUTTON,
                                         BUTTON_PULL,
                                         NRF_GPIO_PIN_SENSE_LOW);

                // Go to system-off mode (this function will not return; wakeup will cause a reset)
                advertising_stop();
            }
            //bluz gateway
            if(p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_SCAN)
            {
            }
            else if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_CONN)
            {
            }
            break;
        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
            system_connection_interval = p_ble_evt->evt.gap_evt.params.conn_param_update.conn_params.max_conn_interval;
            break;
#endif

#if PLATFORM_ID==269
        uint32_t err_code;
        case BLE_GAP_EVT_ADV_REPORT:
        {
            data_t adv_data;
            data_t type_data;
            
//            uint8_t service_uuid[] = {0xB2, 0x2D, 0x14, 0xAA, 0xB3, 0x9F, 0x41, 0xED, 0xB1, 0x77, 0xFF, 0x38, 0x23, 0x02, 0x1E, 0x87};
            
            // Initialize advertisement report for parsing.
            adv_data.p_data = p_ble_evt->evt.gap_evt.params.adv_report.data;
            adv_data.data_len = p_ble_evt->evt.gap_evt.params.adv_report.dlen;
            
            err_code = adv_report_parse(BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME,
                                        &adv_data,
                                        &type_data);
            if (err_code != NRF_SUCCESS)
            {
                // Compare short local name in case complete name does not match.
                err_code = adv_report_parse(BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME,
                                            &adv_data,
                                            &type_data);
            }
            
            //            err_code = adv_report_parse(BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE,
            //                                                  &adv_data,
            //                                                  &type_data);
            
            // Verify if short or complete name matches target.
            if ((err_code == NRF_SUCCESS) &&
                (0 == memcmp(TARGET_DEV_NAME,type_data.p_data,type_data.data_len)) &&
//                isCloudConnected &&
                !isCloudUpdating &&
                system_millis() - lastConnectionTime > TIME_BETWEEN_CONNECTIONS)
            {
                err_code = sd_ble_gap_scan_stop();
                state = BLE_OFF;
                if (err_code != NRF_SUCCESS)
                {
                }

                lastConnectionTime = system_millis();
                err_code = sd_ble_gap_connect(&p_ble_evt->evt.gap_evt.params.adv_report.\
                                              peer_addr,
                                              &m_scan_param,
                                              &m_connection_param);
                
                if (err_code != NRF_SUCCESS)
                {
                    APP_ERROR_CHECK(err_code);
                }
            }
            break;
        }
            
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

        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
            // Accepting parameters requested by peer.
            err_code = sd_ble_gap_conn_param_update(p_ble_evt->evt.gap_evt.conn_handle,
                                                    &p_ble_evt->evt.gap_evt.params.conn_param_update_request.conn_params);
            APP_ERROR_CHECK(err_code);
            break;
#endif

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
#if PLATFORM_ID==269
    dm_ble_evt_handler(p_ble_evt);
    client_handling_ble_evt_handler(p_ble_evt);
#endif

    on_ble_evt(p_ble_evt);
    
#if PLATFORM_ID==103
    ble_conn_params_on_ble_evt(p_ble_evt);
    scs_on_ble_evt(&m_scs, p_ble_evt);
#endif
}

/**@brief Function for handling the Application's system events.
 *
 * @param[in]   sys_evt   system event.
 */
void on_sys_evt(uint32_t sys_evt)
{
#if PLATFORM_ID==269
    switch(sys_evt)
    {
        case NRF_EVT_FLASH_OPERATION_SUCCESS:
        case NRF_EVT_FLASH_OPERATION_ERROR:
            if (m_memory_access_in_progress)
            {
                m_memory_access_in_progress = false;
                gateway_scan_start();
            }
            break;
        default:
            // No implementation needed.
            break;
    }
#endif
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
    on_sys_evt(sys_evt);
}

void data_write_handler(scs_t * p_lbs, uint8_t *data, uint16_t length)
{
    dataManagementFeedData(length, data);
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
#if PLATFORM_ID==269
        uint32_t  err_code;
        case DM_EVT_CONNECTION:
            err_code = client_handling_create(p_handle, p_event->event_param.p_gap_param->conn_handle);
            APP_ERROR_CHECK(err_code);
            m_peer_count++;
            if (m_peer_count < MAX_CLIENTS)
            {
                gateway_scan_start();
            }
            break;
        case DM_EVT_DISCONNECTION:
            nrf_gpio_pin_clear(GATEWAY_NOTIFICATION_LED);

            err_code = client_handling_destroy(p_handle);
            APP_ERROR_CHECK(err_code);

            if (m_peer_count == MAX_CLIENTS)
            {
                gateway_scan_start();
            }
            m_peer_count--;
            break;
        case DM_EVT_SECURITY_SETUP:
        {
            dm_handle_t handle = (*p_handle);
            // Slave securtiy request received from peer, if from a non bonded device,
            // initiate security setup, else, wait for encryption to complete.
            err_code = dm_security_setup_req(&handle);
            APP_ERROR_CHECK(err_code);
            break;
        }
        case DM_EVT_LINK_SECURED:
            break;
        case DM_EVT_DEVICE_CONTEXT_STORED:
            APP_ERROR_CHECK(event_result);
            break;
        case DM_EVT_DEVICE_CONTEXT_DELETED:
            APP_ERROR_CHECK(event_result);
            break;
#endif
        default:
            break;
    }

#if PLATFORM_ID==269
    // Relay the event to client handling module.
    uint32_t err_code = client_handling_dm_event_handler(p_handle, p_event, event_result);
    APP_ERROR_CHECK(err_code);
#endif
    
    return NRF_SUCCESS;
}