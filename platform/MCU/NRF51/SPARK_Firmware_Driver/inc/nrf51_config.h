#include "app_scheduler.h"
#include "app_timer.h"
#include "app_button.h"
#include "app_gpiote.h"
#include "nrf_gpiote.h"

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS            6						                    /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         6                                           /**< Size of timer operation queues. */

#define TIME_KEPPER_INTERVAL     		APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER)     /**< Keep track of time in roughly 1 second intervals. */

#define APP_GPIOTE_MAX_USERS            1                                           /**< Maximum number of users of the GPIOTE handler. */

#define BUTTON_DETECTION_DELAY          APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)    /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */




//timers
app_timer_id_t millis_timer;