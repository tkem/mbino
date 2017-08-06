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
#ifndef MBINO_DIGITAL_IN_H
#define MBINO_DIGITAL_IN_H

#include "platform/platform.h"
#include "hal/gpio_api.h"

namespace mbino {

    class DigitalIn {
    public:
        DigitalIn(PinName pin) {
            gpio_init_in(&gpio, pin);
        }

        DigitalIn(PinName pin, PinMode pull) {
            gpio_init_in(&gpio, pin, pull);
        }

        int read() {
            return gpio_read(&gpio);
        }

        void mode(PinMode pull) {
            gpio_mode(&gpio, pull);
        }

        int is_connected() {
            return gpio_is_connected(&gpio);
        }

        operator int() {
            return read();
        }

    protected:
        gpio_t gpio;

    };

}

#endif
