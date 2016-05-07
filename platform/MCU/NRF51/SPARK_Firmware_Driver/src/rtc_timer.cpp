#include "rtc_timer.h"

RTCTimer::RTCTimer(uint32_t interval, void (*handler)(void), bool one_shot=false)
{
  timerID = 0;
  timerInterval = interval;
  clientHandler = handler;
  timerMode = (one_shot) ? APP_TIMER_MODE_SINGLE_SHOT : APP_TIMER_MODE_REPEATED;
}

RTCTimer::~RTCTimer() { stop(); }

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
  }

}

void RTCTimer::stop()
{
  // TODO
}
