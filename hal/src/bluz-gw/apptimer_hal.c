#include "apptimer_hal.h"
#include "app_timer.h"
#include "nrf51_config.h"

uint32_t HAL_app_timer_create(app_timer_id_t *p_timer_id, app_timer_mode_t mode, app_timer_timeout_handler_t timeout_handler)
{
  return app_timer_create(p_timer_id, mode, timeout_handler);
}

uint32_t HAL_app_timer_start(app_timer_id_t timer_id, uint32_t milliseconds, void *p_context)
{
    uint32_t ticks = MAX(APP_TIMER_TICKS(milliseconds, APP_TIMER_PRESCALER), APP_TIMER_MIN_TIMEOUT_TICKS);
    return app_timer_start(timer_id, ticks, p_context);
}

uint32_t HAL_app_timer_stop(app_timer_id_t timer_id)
{
    return app_timer_stop(timer_id);
}
