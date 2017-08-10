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
#ifndef MBINO_GPIO_API_H
#define MBINO_GPIO_API_H

#include "port_api.h"

// TBD: extern "C"?
namespace mbino {

    typedef port_t gpio_t;

    void gpio_init_in(gpio_t* obj, PinName pin, PinMode pull = PullDefault);

    void gpio_init_out(gpio_t* obj, PinName pin, bool value = false);

    inline void gpio_dir_in(gpio_t* obj, PinMode pull = PullDefault) {
        port_dir_in(obj, pull);
    }

    inline void gpio_dir_out(gpio_t* obj, bool value = false) {
        port_dir_out(obj, value ? obj->mask : 0);
    }

    inline bool gpio_read(gpio_t* obj) {
        return port_read(obj) != 0;
    }

    void gpio_write(gpio_t* obj, bool value);

    inline void gpio_mode(gpio_t* obj, PinMode pull) {
        port_mode(obj, pull);
    }

    inline bool gpio_is_connected(gpio_t* obj) {
        return obj->port != NC;
    }
}

#endif
