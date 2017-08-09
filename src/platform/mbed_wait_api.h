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
#ifndef MBINO_MBED_WAIT_API_H
#define MBINO_MBED_WAIT_API_H

#include "hal/us_ticker_api.h"

namespace mbino {

    inline void wait_us(long us) {
        uint32_t start = us_ticker_read();
        while ((us_ticker_read() - start) < (uint32_t)us)
            ;
    }

    inline void wait_ms(long ms) {
        wait_us(ms * 1000);
    }

    inline void wait(float s) {
        wait_us(s * 1000000.0f);
    }

}

#endif
