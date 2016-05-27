/**
 Copyright (c) 2015 MidAir Technology, LLC.  All rights reserved.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include "bluz_wiring_timer.h"

Timer::~Timer() { dispose(); }

void Timer::start()
{
    if (!timerID) // XXX: app_timer_create crashes if called from the constructor. I think it's because the system isn't ready by then.
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

void Timer::stop()
{
    if (timerID)
    {
        uint32_t err_code = HAL_app_timer_stop(timerID);
        APP_ERROR_CHECK(err_code);
        timerActive = false;
    }
}

bool Timer::isActive()
{
    return timerActive;
}

void Timer::changePeriod(uint32_t interval)
{
    stop();
    timerInterval = interval;
    start();
}

void Timer::reset()
{
    stop();
    start();
};

/*
 * Subclasses can either provide a callback function, or override
 * this timeout method.
 */
void Timer::timeout()
{
    if (callback)
        callback();
}

void Timer::dispose()
{
    stop(); // for compatibility
    /** XXX
     * Nordic's Application Timer does not include a means of completely deleting/de-allocating
     * a timer_id, once it has been created using app_timer_create. If one day needed (unlikely)
     * then it should suffice to add a new _DELETE enum value for timer_user_op_type_t and have
     * list_deletions_handler() do a  [mp_nodes[timer_id].state=FREE], after processing _STOP.
     **/

}

void Timer::staticCallback(void *context)
{
    Timer *instance = static_cast<Timer *>(context);
    instance->timeout();
};

