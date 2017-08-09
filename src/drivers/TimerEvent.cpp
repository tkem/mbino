/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
 *
 * mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You
 * may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.  See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include "TimerEvent.h"

#include "hal/us_ticker_api.h"

namespace mbino {

    TimerEvent::TimerEvent() : _ticker_data(get_us_ticker_data())
    {
        ticker_init_event(&event, &TimerEvent::irq, (intptr_t)this);
    }

    TimerEvent::TimerEvent(const ticker_data_t *data) : _ticker_data(data)
    {
        ticker_init_event(&event, &TimerEvent::irq, (intptr_t)this);
    }

    void TimerEvent::irq(intptr_t id)
    {
        TimerEvent* timer_event = reinterpret_cast<TimerEvent*>(id);
        timer_event->handler();
    }

}
