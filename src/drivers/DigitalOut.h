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
#ifndef MBINO_DIGITAL_OUT_H
#define MBINO_DIGITAL_OUT_H

#include "platform/platform.h"
#include "hal/gpio_api.h"

namespace mbino {

    class DigitalOut {
    public:
        DigitalOut(PinName pin) {
            gpio_init_out(&gpio, pin);
        }

        DigitalOut(PinName pin, int value) {
            gpio_init_out(&gpio, pin, value != 0);
        }

        void write(int value) {
            gpio_write(&gpio, value != 0);
        }

        int read() {
            return gpio_read(&gpio);
        }

        int is_connected() {
            return gpio_is_connected(&gpio);
        }

        DigitalOut& operator=(uint8_t value) {
            write(value);
            return *this;
        }

        DigitalOut& operator=(DigitalOut& rhs) {
            write(rhs.read());
            return *this;
        }

        operator int() {
            return read();
        }

    protected:
        gpio_t gpio;

    };

}

#endif
