#include "nrf51_callbacks.h"
#include "data_management_layer.h"
#include "debug.h"

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
}

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
void on_ble_evt(ble_evt_t * p_ble_evt)
{
    DEBUG("Received BLE Event!");
    uint32_t                         err_code;
    static ble_gap_evt_auth_status_t m_auth_status;
    ble_gap_enc_info_t *             p_enc_info;
    ble_gap_addr_t* address = 0;
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            nrf_gpio_pin_clear(BOARD_LED_PIN);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            
            sd_ble_gap_address_get(address);
            
            state = BLE_CONNECTED;
            break;
            
        case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            
            err_code = app_button_disable();
            APP_ERROR_CHECK(err_code);
            
            advertising_start();
            state = BLE_ADVERTISING;
            break;
            
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle,
                                                   BLE_GAP_SEC_STATUS_SUCCESS,
                                                   &m_sec_params);
            APP_ERROR_CHECK(err_code);
            break;
            
        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0);
            APP_ERROR_CHECK(err_code);
            break;
            
        case BLE_GAP_EVT_AUTH_STATUS:
            m_auth_status = p_ble_evt->evt.gap_evt.params.auth_status;
            break;
            
        case BLE_GAP_EVT_SEC_INFO_REQUEST:
            p_enc_info = &m_auth_status.periph_keys.enc_info;
            if (p_enc_info->div == p_ble_evt->evt.gap_evt.params.sec_info_request.div)
            {
                err_code = sd_ble_gap_sec_info_reply(m_conn_handle, p_enc_info, NULL);
                APP_ERROR_CHECK(err_code);
            }
            else
            {
                // No keys found for this device
                err_code = sd_ble_gap_sec_info_reply(m_conn_handle, NULL, NULL);
                APP_ERROR_CHECK(err_code);
            }
            break;
            
        case BLE_GAP_EVT_TIMEOUT:
            if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISEMENT)
            {
                nrf_gpio_pin_clear(BOARD_LED_PIN);
                
                // Configure buttons with sense level low as wakeup source.
                nrf_gpio_cfg_sense_input(BOARD_BUTTON,
                                         BUTTON_PULL,
                                         NRF_GPIO_PIN_SENSE_LOW);
                
                // Go to system-off mode (this function will not return; wakeup will cause a reset)
                err_code = sd_power_system_off();
                APP_ERROR_CHECK(err_code);
            }
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
    //pstorage_sys_event_handler(sys_evt);
}

void data_write_handler(scs_t * p_lbs, uint8_t *data, uint16_t length)
{
    DEBUG("Received Data from Gateway!");
    //TO DO: This needs to feed data into Spark Utilities library
    uint16_t serviceID = (data[0] << 8) | data[1];
    dataManagementFeedData(serviceID, length-2, data+2);

    
    //    BLESocketFeedData(data, length);
//    if (length == 40) {
//        if (!handShook) {
//            doHandshake = true;
//        }
//    } else {
//        simple_uart_putstring("checking if we should run the loop  \n");
//        if (cloudConnected && !Spark_Communication_Loop()) {
//            simple_uart_putstring("Caught an error when looping  \n");
//        }
//    }
}