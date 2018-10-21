/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
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
#ifdef ARDUINO_ARCH_SAMD

#include <Arduino.h>

#include "hal/port_api.h"

static void port_dir_in(port_t *obj, PinMode pull)
{
    // FIXME: pullup/down(?)
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    PORT->Group[obj->port].DIRSET.reg &= ~obj->mask;
    __set_PRIMASK(primask);
}

static void port_dir_out(port_t *obj, int value)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    PORT->Group[obj->port].OUTCLR.reg = obj->mask;
    PORT->Group[obj->port].OUTSET.reg = (value & obj->mask);
    PORT->Group[obj->port].DIRSET.reg |= obj->mask;
    __set_PRIMASK(primask);
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    obj->port = port;
    obj->mask = mask;

    if (dir == PIN_INPUT) {
        port_dir_in(obj, PullDefault);
    } else {
        port_dir_out(obj, 0);
    }
}

void port_mode(port_t *obj, PinMode mode)
{
    // FIXME: pullup/down(?)
}

void port_dir(port_t *obj, PinDirection dir)
{
    // FIXME: mode/value
    if (dir == PIN_INPUT) {
        port_dir_in(obj, PullDefault);
    } else {
        port_dir_out(obj, 0);
    }
}

void port_write(port_t *obj, int value)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    PORT->Group[obj->port].OUTCLR.reg = (~value & obj->mask);
    PORT->Group[obj->port].OUTSET.reg = (value & obj->mask);
    __set_PRIMASK(primask);
}

int port_read(port_t *obj)
{
    return PORT->Group[obj->port].IN.reg & obj->mask;
}

#endif
