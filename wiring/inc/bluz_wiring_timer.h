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

#ifndef __BLUZ_WIRING_TIMER_H
#define	__BLUZ_WIRING_TIMER_H


#include <stdint.h>
#include <stddef.h> // NULL
#include <functional>
#include "apptimer_hal.h"

#ifdef __cplusplus
extern "C" {
#endif


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

    ~Timer();
    void start();
    void stop();
    bool isActive();
    void changePeriod(uint32_t interval);
    void reset();
    
    /*
     * Subclasses can either provide a callback function, or override
     * this timeout method.
     */
    virtual void timeout();
    void dispose();

  private:
    int timerID;
    uint32_t timerInterval;
    volatile bool timerActive;
    app_timer_mode_t timerMode;
    timer_callback_fn callback;
    
    static void staticCallback(void *context);
};

#endif /* __RTC_TIMER_H */
