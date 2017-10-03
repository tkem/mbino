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
#ifdef ARDUINO_ARCH_AVR

#include "hal/port_api.h"

#include <Arduino.h>

void port_init_in_ex(port_t *obj, PortName port, int mask, PinMode mode)
{
    obj->port = port;
    obj->mask = mask;
    port_dir_in(obj, mode);
}

void port_init_out_ex(port_t *obj, PortName port, int mask, int value)
{
    obj->port = port;
    obj->mask = mask;
    port_dir_out(obj, value);
}

void port_dir_in(port_t *obj, PinMode pull)
{
    uint8_t data = pull == PullUp ? obj->mask : 0;
    volatile uint8_t *mode = portModeRegister(obj->port);
    volatile uint8_t *input = portInputRegister(obj->port);
    volatile uint8_t *output = portOutputRegister(obj->port);

    uint8_t sreg = SREG;
    cli();
    obj->reg = input;
    *mode &= ~obj->mask;
    *output = (*output & ~obj->mask) | data;
    SREG = sreg;
}

void port_dir_out(port_t *obj, int value)
{
    uint8_t data = value & obj->mask;
    volatile uint8_t *mode = portModeRegister(obj->port);
    volatile uint8_t *output = portOutputRegister(obj->port);

    uint8_t sreg = SREG;
    cli();
    obj->reg = output;
    *output = (*output & ~obj->mask) | data;
    *mode |= obj->mask;
    SREG = sreg;
}

int port_read(port_t *obj)
{
    return *obj->reg & obj->mask;
}

void port_write(port_t *obj, int value)
{
    uint8_t sreg = SREG;
    cli();
    *obj->reg = (*obj->reg & ~obj->mask) | (value & obj->mask);
    SREG = sreg;
}

void port_mode(port_t *obj, PinMode mode)
{
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

#endif
