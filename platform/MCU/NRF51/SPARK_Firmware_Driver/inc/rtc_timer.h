#ifndef __RTC_TIMER_H_
#define	__RTC_TIMER_H_
#include <stdint.h>
#include <stddef.h> // NULL

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_error.h"
#include "app_error.h"

#ifdef __cplusplus
}
#endif

#include "apptimer_hal.h"

class RTCTimer
{

  public:
    RTCTimer(uint32_t interval, void (*handler)(void), bool one_shot);
    ~RTCTimer();
    void start();
    void stop();

  private:
    int timerID;
    uint32_t timerInterval;
    app_timer_mode_t timerMode;
    void (*clientHandler)(void);
    
    static void staticHandler(void *context)
    {
        RTCTimer *instance = static_cast<RTCTimer *>(context);
        if (instance->clientHandler) instance->clientHandler();
    };

};

#endif /* __RTC_TIMER_H */
