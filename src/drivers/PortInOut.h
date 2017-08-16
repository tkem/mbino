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
#ifndef MBINO_PORT_IN_OUT_H
#define MBINO_PORT_IN_OUT_H

#include "platform/platform.h"

#include "hal/port_api.h"

namespace mbino {

    class PortInOut {
        port_t _port;
        PinMode _mode;
        int _value;

    public:

        PortInOut(PortName port, int mask = ~0);

        void write(int value);

        int read() {
            return port_read(&_port);
        }

        void output() {
            port_dir_out(&_port, _value);
        }

        void input() {
            port_dir_in(&_port, _mode);
        }

        void mode(PinMode mode);

        PortInOut& operator=(int value) {
            write(value);
            return *this;
        }

        PortInOut& operator=(PortInOut& rhs);

        operator int() {
            return read();
        }
    };

}

#endif
