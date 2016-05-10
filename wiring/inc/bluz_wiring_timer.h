#ifndef __RTC_TIMER_H_
#define	__RTC_TIMER_H_
#include <stdint.h>
#include <stddef.h> // NULL
#include <functional>

#ifdef __cplusplus
extern "C" {
#endif
#include "nrf_error.h"
#include "app_error.h"
#include "apptimer_hal.h"
#ifdef __cplusplus
}
#endif

class Timer
{

  public:

    typedef std::function<void(void)> timer_callback_fn;

    // for when standard C function provided as the callback
    Timer(uint32_t interval, timer_callback_fn callback_, bool one_shot=false) :
      timerID(0), 
      timerInterval(interval),
      timerActive(false), 
      timerMode((one_shot) ? APP_TIMER_MODE_SINGLE_SHOT : APP_TIMER_MODE_REPEATED),
      callback(std::move(callback_))
    {
    }

    // for when class method provided as the callback
    template <typename T>
    Timer(uint32_t interval, void (T::*handler)(), T& instance, bool one_shot=false) :
      Timer(interval, std::bind(handler, &instance), one_shot)
    {
    }

    ~Timer() { dispose(); }

    void start() 
    {
      if (!timerID) // XXX: app_timer_create crashes if called from the contructor. I think it's because the system isn't ready by then.
      {
        app_timer_id_t timer_id;
        uint32_t err_code = HAL_app_timer_create( &timer_id, timerMode, staticCallback );
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

    void changePeriod(uint32_t interval)
    {
      stop();
      timerInterval = interval;
      start();
    }

    void reset()
    {
      stop();
      start();
    };
    
    /*
     * Subclasses can either provide a callback function, or override
     * this timeout method.
     */
    virtual void timeout()
    {
      if (callback) 
        callback(); 
    }

    void dispose()
    {
      stop(); // for compatibility
      /** XXX
       * Nordic's Application Timer does not include a means of completely deleting/de-allocating 
       * a timer_id, once it has been created using app_timer_create. If one day needed (unlikely)
       * then it should suffice to add a new _DELETE enum value for timer_user_op_type_t and have
       * list_deletions_handler() do a  [mp_nodes[timer_id].state=FREE], after processing _STOP.
       **/

    }

  private:
    int timerID;
    uint32_t timerInterval;
    volatile bool timerActive;
    app_timer_mode_t timerMode;
    timer_callback_fn callback;
    
    static void staticCallback(void *context)
    {
        Timer *instance = static_cast<Timer *>(context);
        instance->timeout();
    };

};

#endif /* __RTC_TIMER_H */
