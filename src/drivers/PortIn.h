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
#ifndef MBINO_PORT_IN_H
#define MBINO_PORT_IN_H

#include "platform/platform.h"

#if defined(DEVICE_PORTIN) || defined(DOXYGEN_ONLY)

#include "hal/port_api.h"

namespace mbino {

    class PortIn {
        port_t _port;

    public:

        PortIn(PortName port, int mask = ~0) {
            port_init_in(&_port, port, mask);
        }

        int read() {
            return port_read(&_port);
        }

        void mode(PinMode pull) {
            port_mode(&_port, pull);
        }

        operator int() {
            return read();
        }
    };

}

#endif

#endif
