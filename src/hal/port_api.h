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
#ifndef MBINO_PORT_API_H
#define MBINO_PORT_API_H

#include "platform/platform.h"

namespace mbino {

    struct port_t {
        volatile uint8_t* reg;
        uint8_t port;
        uint8_t mask;
    };

    void port_init_in(port_t* obj, PortName port, uint8_t mask, PinMode pull = PullDefault);

    void port_init_out(port_t* obj, PortName port, uint8_t mask, uint8_t value = 0);

    void port_dir_in(port_t* obj, PinMode pull = PullDefault);

    void port_dir_out(port_t* obj, uint8_t value = 0);

    int port_read(port_t* obj);

    void port_write(port_t* obj, uint8_t value);

    void port_mode(port_t* obj, PinMode pull);

}

#endif
