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
#ifndef MBINO_ANALOG_IN_H
#define MBINO_ANALOG_IN_H

#include "platform/platform.h"

#if defined (DEVICE_ANALOGIN) || defined(DOXYGEN_ONLY)

#include "hal/analogin_api.h"

namespace mbino {

    class AnalogIn {
        analogin_t _adc;

    public:

        AnalogIn(PinName pin) {
            analogin_init(&_adc, pin);
        }

        float read() {
            return analogin_read(&_adc);
        }

        unsigned short read_u16() {
            return analogin_read_u16(&_adc);
        }

        operator float() {
            return read();
        }

    protected:
        /* mbino restriction: no lock methods
        virtual void lock();
        virtual void unlock();
        */
    };

}

#endif

#endif
