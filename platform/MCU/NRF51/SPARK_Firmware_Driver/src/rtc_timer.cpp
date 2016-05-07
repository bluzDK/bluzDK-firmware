#include "rtc_timer.h"

RTCTimer::RTCTimer(uint32_t interval, void (*handler)(void), bool one_shot=false)
{
  timerID = 0;
  timerInterval = interval;
  clientHandler = handler;
  timerActive = false;
  timerMode = (one_shot) ? APP_TIMER_MODE_SINGLE_SHOT : APP_TIMER_MODE_REPEATED;
}

RTCTimer::~RTCTimer() { dispose(); }

void RTCTimer::start() 
{
  if (!timerID)
  {
    app_timer_id_t timer_id;
    uint32_t err_code = HAL_app_timer_create( &timer_id, timerMode, RTCTimer::staticHandler );
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

void RTCTimer::stop()
{
  if (timerID)
  {
    uint32_t err_code = HAL_app_timer_stop(timerID);
    APP_ERROR_CHECK(err_code);
    timerActive = false;
  }
}

bool RTCTimer::isActive()
{
  return (timerActive);
}

void RTCTimer::dispose()
{
  stop();
  // TODO: Not sure if there even is a way to delete an app_timer. Refer to Nordic's docs.
}
