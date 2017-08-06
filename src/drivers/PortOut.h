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
#ifndef MBINO_PORT_OUT_H
#define MBINO_PORT_OUT_H

#include "platform/platform.h"

#include "hal/port_api.h"

namespace mbino {

    class PortOut {
        port_t _port;

    public:
        PortOut(PortName port, uint8_t mask = 0xFF) {
            port_init_out(&_port, port, mask);
        }

        void write(int value) {
            port_write(&_port, value);
        }

        int read() {
            return port_read(&_port);
        }

        PortOut& operator=(int value) {
            write(value);
            return *this;
        }

        PortOut& operator=(PortOut& rhs) {
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
