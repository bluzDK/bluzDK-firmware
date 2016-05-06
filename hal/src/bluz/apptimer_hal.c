#include "apptimer_hal.h"

uint32_t HAL_app_timer_create(app_timer_id_t *p_timer_id, app_timer_mode_t mode, app_timer_timeout_handler_t timeout_handler)
{
  return app_timer_create(p_timer_id, mode, timeout_handler);
}

uint32_t HAL_app_timer_start(app_timer_id_t timer_id, uint32_t timeout_ticks, void * p_context)
{
  return app_timer_start(timer_id, timeout_ticks, p_context);
}

