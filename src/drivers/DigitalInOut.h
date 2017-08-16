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
#ifndef MBINO_DIGITAL_IN_OUT_H
#define MBINO_DIGITAL_IN_OUT_H

#include "platform/platform.h"

#include "hal/gpio_api.h"

namespace mbino {

    class DigitalInOut {
        PinMode _mode;
        bool _value;

    public:

        DigitalInOut(PinName pin);

        DigitalInOut(PinName pin, PinDirection direction, PinMode mode, int value);

        void write(int value);

        int read() {
            return gpio_read(&gpio);
        }

        void output() {
            gpio_dir_out(&gpio, _value);
        }

        void input() {
            gpio_dir_in(&gpio, _mode);
        }

        void mode(PinMode mode);

        int is_connected() {
            return gpio_is_connected(&gpio);
        }

        DigitalInOut& operator=(int value) {
            write(value);
            return *this;
        }

        DigitalInOut& operator=(DigitalInOut& rhs);

        operator int() {
            return read();
        }

    protected:
        gpio_t gpio;
    };

}

#endif
