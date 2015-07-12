#include <stdint.h>
#include "hw_config.h"

void button_event_handler(uint8_t pin_no, uint8_t button_action);
void millis_timer_timeout(void * p_context);