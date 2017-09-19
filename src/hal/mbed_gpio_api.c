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
#include "gpio_api.h"

void gpio_init(gpio_t *obj, PinName pin)
{
    gpio_init_in(obj, pin);
}

void gpio_init_in(gpio_t *obj, PinName pin)
{
    gpio_init_in_ex(obj, pin, PullDefault);
}

void gpio_init_out(gpio_t *obj, PinName pin)
{
    gpio_init_out_ex(obj, pin, 0);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    if (direction == PIN_INPUT) {
        gpio_dir_in(obj, PullDefault);
    } else {
        gpio_dir_out(obj, 0);
    }
}
