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

#include "platform/NonCopyable.h"

#include <Arduino.h>

namespace mbino {

    class Timer : private NonCopyable<Timer> {
        unsigned long _micros;
        // TODO: running, extended us interval?

    public:
        void start() {
            _micros = micros();
        }

        void stop() {
            // TODO: running
        }

        void reset() {
            _micros = micros();
        }

        float read() {
            return read_us() / 1000000.f;
        }

        unsigned long read_ms() {
            return read_us() / 1000;
        }

        unsigned long read_us() {
            return micros() - _micros;
        }

        operator float() {
            return read();
        }

        unsigned long read_high_resolution_us() {
            // TODO: improved resolution?
            return read_us();
        }
    };

}

#endif
