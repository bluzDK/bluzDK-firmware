#include "nrf51_callbacks.h"

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