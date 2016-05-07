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

    RTCTimer(uint32_t interval, void (*handler)(void), bool one_shot=false)
    {
      timerID = 0;
      timerInterval = interval;
      clientHandler = handler;
      timerActive = false;
      timerMode = (one_shot) ? APP_TIMER_MODE_SINGLE_SHOT : APP_TIMER_MODE_REPEATED;
    }

    ~RTCTimer() { dispose(); }

    void start() 
    {
      if (!timerID)
      {
        app_timer_id_t timer_id;
        uint32_t err_code = HAL_app_timer_create( &timer_id, timerMode, staticHandler );
        APP_ERROR_CHECK(err_code);
        timerID = timer_id;
      }

      if (timerID)
      {
        uint32_t err_code = HAL_app_timer_start(timerID, timerInterval, this);
        APP_ERROR_CHECK(err_code);
        timerActive = true;
      }

    }

    void stop()
    {
      if (timerID)
      {
        uint32_t err_code = HAL_app_timer_stop(timerID);
        APP_ERROR_CHECK(err_code);
        timerActive = false;
      }
    }

    bool isActive()
    {
      return (timerActive);
    }

    void reset()
    {
    };
    
    void changePeriod()
    {
    }

    void dispose()
    {
      stop();
      // TODO: Not sure if there even is a way to delete an app_timer. Refer to Nordic's docs.
    }

  private:
    int timerID;
    uint32_t timerInterval;
    app_timer_mode_t timerMode;
    void (*clientHandler)(void);
    volatile bool timerActive;
    
    static void staticHandler(void *context)
    {
        RTCTimer *instance = static_cast<RTCTimer *>(context);
        if (instance->clientHandler) instance->clientHandler();
    };

};

#endif /* __RTC_TIMER_H */
