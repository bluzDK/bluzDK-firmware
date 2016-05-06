#ifndef APP_TIMER_HAL_H
#define APP_TIMER_HAL_H

#include <stdint.h>
#include "dynalib.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef APP_TIMER_H__
typedef uint32_t app_timer_id_t;
typedef void (*app_timer_timeout_handler_t)(void * p_context);
typedef enum
{
    APP_TIMER_MODE_SINGLE_SHOT,                 // The timer will expire only once.
    APP_TIMER_MODE_REPEATED                     // The timer will restart each time it expires.
} app_timer_mode_t;

extern uint32_t app_timer_create(app_timer_id_t *p_timer_id, app_timer_mode_t mode, app_timer_timeout_handler_t timeout_handler);
extern uint32_t app_timer_start(app_timer_id_t timer_id, uint32_t timeout_ticks, void * p_context);
#endif

uint32_t HAL_app_timer_create(app_timer_id_t *p_timer_id, app_timer_mode_t mode, app_timer_timeout_handler_t timeout_handler);
uint32_t HAL_app_timer_start(app_timer_id_t timer_id, uint32_t timeout_ticks, void * p_context);

#ifdef __cplusplus
}
#endif

#endif


