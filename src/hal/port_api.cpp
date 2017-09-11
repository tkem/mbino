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
#include "port_api.h"

#include <Arduino.h>

namespace mbino {

    // TODO: handle NOT_A_PORT/NC

    static void port_dir(port_t* obj, PinDirection dir, uint8_t data)
    {
        volatile uint8_t* mode = portModeRegister(obj->port);
        volatile uint8_t* input = dir == PIN_INPUT ? portInputRegister(obj->port) : 0;
        volatile uint8_t* output = portOutputRegister(obj->port);

        uint8_t sreg = SREG;
        cli();
        uint8_t out = (*output & ~obj->mask) | (data & obj->mask);
        if (dir == PIN_INPUT) {
            obj->reg = input;
            *mode &= ~obj->mask;
            *output = out;
        } else {
            obj->reg = output;
            *output = out;
            *mode |= obj->mask;
        }
        SREG = sreg;
    }

    static void port_init(port_t* obj, PortName port, uint8_t mask, PinDirection dir, uint8_t data)
    {
        obj->port = port;
        obj->mask = mask;
        port_dir(obj, dir, data);
    }

    void port_init_in(port_t* obj, PortName port, uint8_t mask, PinMode pull)
    {
        port_init(obj, port, mask, PIN_INPUT, pull == PullUp ? mask : 0);
    }

    void port_init_out(port_t* obj, PortName port, uint8_t mask, uint8_t value)
    {
        port_init(obj, port, mask, PIN_OUTPUT, value);
    }

    void port_dir_in(port_t* obj, PinMode pull)
    {
        port_dir(obj, PIN_INPUT, pull == PullUp ? obj->mask : 0);
    }

    void port_dir_out(port_t* obj, uint8_t value)
    {
        port_dir(obj, PIN_OUTPUT, value);
    }

    int port_read(port_t* obj)
    {
        return *obj->reg & obj->mask;
    }

    void port_write(port_t* obj, uint8_t value)
    {
        uint8_t sreg = SREG;
        cli();
        *obj->reg = (*obj->reg & ~obj->mask) | (value & obj->mask);
        SREG = sreg;
    }

    void port_mode(port_t* obj, PinMode pull)
    {
        volatile uint8_t* output = portOutputRegister(obj->port);

        uint8_t sreg = SREG;
        cli();
        if (pull == PullUp) {
            *output |= obj->mask;
        } else {
            *output &= obj->mask;
        }
        SREG = sreg;
    }

}
