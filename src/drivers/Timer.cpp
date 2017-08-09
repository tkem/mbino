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
#include "Timer.h"

#include "hal/us_ticker_api.h"

#include <Arduino.h>

namespace mbino {

    Timer::Timer() : _ticker_data(get_us_ticker_data())
    {
        _time = 0;
        _running = false;
    }

    Timer::Timer(const ticker_data_t* data) : _ticker_data(data)
    {
        _time = 0;
        _running = false;
    }

    void Timer::start()
    {
        uint8_t sreg = SREG;
        cli();
        if (!_running) {
            _start = ticker_read_us(_ticker_data);
            _running = true;
        }
        SREG = sreg;
    }

    void Timer::stop() {
        uint8_t sreg = SREG;
        cli();
        _time = ticker_read_us(_ticker_data) - _start;
        _running = false;
        SREG = sreg;
    }

    void Timer::reset() {
        uint8_t sreg = SREG;
        cli();
        _start = ticker_read_us(_ticker_data);
        SREG = sreg;
    }

    long Timer::read_us() {
        // only 32 bits needed
        timestamp_t time;
        uint8_t sreg = SREG;
        cli();
        if (_running) {
            time = ticker_read(_ticker_data) - static_cast<timestamp_t>(_start);
        } else {
            time = _time;
        }
        SREG = sreg;
        return time;
    }

    us_timestamp_t Timer::read_high_resolution_us() {
        us_timestamp_t time;
        uint8_t sreg = SREG;
        cli();
        if (_running) {
            time = ticker_read_us(_ticker_data) - _start;
        } else {
            time = _time;
        }
        SREG = sreg;
        return time;
    }

}
