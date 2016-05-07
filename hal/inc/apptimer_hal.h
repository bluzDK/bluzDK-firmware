#ifndef APP_TIMER_HAL_H
#define APP_TIMER_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "app_timer.h"

uint32_t HAL_app_timer_create(app_timer_id_t *p_timer_id, app_timer_mode_t mode, app_timer_timeout_handler_t timeout_handler);
uint32_t HAL_app_timer_start(app_timer_id_t timer_id, uint32_t milliseconds, void * p_context);
uint32_t HAL_app_timer_stop(app_timer_id_t timer_id);

#ifdef __cplusplus
}
#endif

#endif


