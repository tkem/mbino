/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
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
#ifdef ARDUINO_ARCH_AVR

#include <Arduino.h>

#include "hal/port_api.h"

static void port_dir_in(port_t *obj, PinMode pull)
{
    volatile uint8_t *mode = portModeRegister(obj->port);
    volatile uint8_t *input = portInputRegister(obj->port);
    volatile uint8_t *output = portOutputRegister(obj->port);
    uint8_t data = pull == PullUp ? obj->mask : 0;

    obj->reg = input;

    uint8_t sreg = SREG;
    cli();
    *mode &= ~obj->mask;
    *output = (*output & ~obj->mask) | data;
    SREG = sreg;
}

static void port_dir_out(port_t *obj, int value)
{
    volatile uint8_t *mode = portModeRegister(obj->port);
    volatile uint8_t *output = portOutputRegister(obj->port);
    uint8_t data = value & obj->mask;

    obj->reg = output;

    uint8_t sreg = SREG;
    cli();
    *output = (*output & ~obj->mask) | data;
    *mode |= obj->mask;
    SREG = sreg;
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
    if (obj->dir == PIN_INPUT) {
        volatile uint8_t *output = portOutputRegister(obj->port);

        uint8_t sreg = SREG;
        cli();
        if (mode == PullUp) {
            *output |= obj->mask;
        } else {
            *output &= obj->mask;
        }
        SREG = sreg;
    }
    obj->mode = mode;
}

void port_dir(port_t *obj, PinDirection dir)
{
    if (dir == PIN_INPUT) {
        port_dir_in(obj, obj->mode);
    } else {
        port_dir_out(obj, obj->value);
    }
}

void port_write(port_t *obj, int value)
{
    obj->value = value & obj->mask;
    if (obj->dir == PIN_OUTPUT) {
        uint8_t sreg = SREG;
        cli();
        *obj->reg = (*obj->reg & ~obj->mask) | obj->value;
        SREG = sreg;
    }
}

int port_read(port_t *obj)
{
    return *obj->reg & obj->mask;
}

#endif
