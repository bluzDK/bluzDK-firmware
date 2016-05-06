#include "apptimer_hal.h"

// point HAL version at some existing SD functions
uint32_t (*HAL_app_timer_create)(app_timer_id_t *p_timer_id, app_timer_mode_t mode, app_timer_timeout_handler_t timeout_handler) = app_timer_create;
uint32_t (*HAL_app_timer_start)(app_timer_id_t timer_id, uint32_t timeout_ticks, void * p_context) = app_timer_start;
uint32_t (*HAL_app_timer_stop)(app_timer_id_t timer_id) = app_timer_stop;
