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
#ifndef MBINO_ANALOG_OUT_H
#define MBINO_ANALOG_OUT_H

#include "platform/platform.h"

namespace mbino {

    class AnalogOut {
        PinName _pin;

    public:
        AnalogOut(PinName pin) : _pin(pin) {
            pinMode(pin, OUTPUT);
        }

        void write(float value) {
            if (value < 0.0f) {
                analogWrite(_pin, 0);
            } else if (value > 1.0f) {
                analogWrite(_pin, 255);
            } else {
                analogWrite(_pin, uint8_t(value * 255.0f));
            }
        }

        void write_u16(uint16_t value) {
            analogWrite(_pin, value >> 8);
        }

        float read() {
            return float(analogRead(_pin)) * (1.0f / float(0x3FF));
        }

        AnalogOut& operator=(float value) {
            write(value);
            return *this;
        }

        AnalogOut& operator=(AnalogOut& rhs) {
            write(rhs.read());
            return *this;
        }
    };

}

#endif
