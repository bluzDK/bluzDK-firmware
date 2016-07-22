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

#include <stdint.h>
#include "hw_config.h"
#include "app_uart.h"

void uart_error_handle(app_uart_evt_t * p_event);
void conn_params_error_handler(uint32_t nrf_error);

void button_event_handler(uint8_t pin_no, uint8_t button_action);
void millis_timer_timeout(void * p_context);
void micros_timer_timeout(nrf_timer_event_t event_type, void* p_context);
void wdt_event_handler(void);
void ble_evt_dispatch(ble_evt_t * p_ble_evt);
void sys_evt_dispatch(uint32_t sys_evt);
uint32_t device_manager_evt_handler(dm_handle_t const    * p_handle,
                                           dm_event_t const     * p_event,
                                           ret_code_t           event_result);

//internal functions
void on_ble_evt(ble_evt_t * p_ble_evt);
void data_write_handler(scs_t * p_lbs, uint8_t *data, uint16_t length);