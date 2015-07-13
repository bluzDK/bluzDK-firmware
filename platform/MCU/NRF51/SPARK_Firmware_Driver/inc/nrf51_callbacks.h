#include <stdint.h>
#include "hw_config.h"

void button_event_handler(uint8_t pin_no, uint8_t button_action);
void millis_timer_timeout(void * p_context);
void ble_evt_dispatch(ble_evt_t * p_ble_evt);
void sys_evt_dispatch(uint32_t sys_evt);

//internal functions
void on_ble_evt(ble_evt_t * p_ble_evt);
void data_write_handler(scs_t * p_lbs, uint8_t *data, uint16_t length);