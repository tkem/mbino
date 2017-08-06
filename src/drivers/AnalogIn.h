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

namespace mbino {

    class AnalogIn {
        PinName _pin;

    public:
        AnalogIn(PinName pin) : _pin(pin) {
            pinMode(pin, INPUT);
        }

        float read() {
            return float(analogRead(_pin)) * (1.0f / float(0x3FF));
        }

        uint16_t read_u16() {
            uint16_t value = analogRead(_pin);
            // 10-bit to 16-bit conversion
            return (value << 6) | (value >> 4);
        }

        operator float() {
            return read();
        }
    };

}

#endif
