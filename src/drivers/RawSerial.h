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
#ifndef MBINO_RAW_SERIAL_H
#define MBINO_RAW_SERIAL_H

#include "platform/platform.h"
#include "platform/NonCopyable.h"

#include "SerialBase.h"

#ifdef putc
#undef putc
#endif

#ifdef puts
#undef puts
#endif

namespace mbino {

    class RawSerial: public SerialBase, private NonCopyable<RawSerial> {
    public:
        RawSerial(PinName tx, PinName rx, int baud = 9600) : SerialBase(tx, rx, baud) {}

        RawSerial(USBTX_type tx, USBRX_type rx, long baud = 9600) : SerialBase(tx, rx, baud) {}

        int getc() {
            return _base_getc();
        }

        int putc(int c) {
            return _base_putc(c);
        }

        int puts(const char* str) {
            return _base_puts(str);
        }

        int printf(const char *format, ...);
    };

}

#endif
