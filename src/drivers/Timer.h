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
#ifndef MBINO_TIMER_H
#define MBINO_TIMER_H

#include "platform/platform.h"

#include "hal/ticker_api.h"
#include "platform/NonCopyable.h"

namespace mbino {
    class Timer : private NonCopyable<Timer> {
        const ticker_data_t *_ticker_data;
        us_timestamp_t _start;
        us_timestamp_t _time;
        bool _running;

    public:
        Timer();

        Timer(const ticker_data_t* data);

        void start();

        void stop();

        void reset();

        float read() {
            return read_high_resolution_us() / 1000000.0f;
        }

        long read_ms() {
            return read_high_resolution_us() / 1000;
        }

        long read_us();

        us_timestamp_t read_high_resolution_us();

        operator float() {
            return read();
        }
    };

}

#endif
