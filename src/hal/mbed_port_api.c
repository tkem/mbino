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
#include "port_api.h"

#if DEVICE_PORTIN || DEVICE_PORTOUT

void port_init(port_t *obj, PortName port, int mask, PinDirection direction)
{
    if (direction == PIN_INPUT) {
        port_init_in(obj, port, mask);
    } else {
        port_init_out(obj, port, mask);
    }
}

void port_init_in(port_t *obj, PortName port, int mask)
{
    port_init_in_ex(obj, port, mask, PullDefault);
}

void port_init_out(port_t *obj, PortName port, int mask)
{
    port_init_out_ex(obj, port, mask, 0);
}

void port_dir(port_t *obj, PinDirection direction)
{
    if (direction == PIN_INPUT) {
        port_dir_in(obj, PullDefault);
    } else {
        port_dir_out(obj, 0);
    }
}

#endif
