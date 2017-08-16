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
#ifndef MBINO_TIMER_EVENT_H
#define MBINO_TIMER_EVENT_H

#include "hal/ticker_api.h"
#include "platform/NonCopyable.h"

namespace mbino {

    class TimerEvent : private NonCopyable<TimerEvent> {
    public:

        TimerEvent();

        TimerEvent(const ticker_data_t *data);

        virtual ~TimerEvent() {
            remove();
        }

    protected:

        virtual void handler() = 0;

        void insert(timestamp_t timestamp) {
            ticker_insert_event(_ticker_data, &event, timestamp);
        }

        void insert_absolute(us_timestamp_t timestamp) {
            ticker_insert_event_us(_ticker_data, &event, timestamp);
        }

        void remove() {
            ticker_remove_event(_ticker_data, &event);
        }

        ticker_event_t event;

        const ticker_data_t *_ticker_data;

    protected:
        static void irq(intptr_t id);
    };

}

#endif
