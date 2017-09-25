/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
 *
 * mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.  See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include "RawSerial.h"

#ifdef DEVICE_SERIAL

#include <alloca.h>
#include <stdarg.h>

#define STRING_STACK_LIMIT 80

namespace mbino {

    static int rsnprintf(RawSerial* obj, size_t n, const char* format, va_list arg)
    {
        char* buf = static_cast<char*>(alloca(n));
        int len = vsnprintf(buf, n, format, arg);
        if (len >= 0 && size_t(len) < n) {
            obj->puts(buf);
        }
        return len;
    }

    int RawSerial::puts(const char* str)
    {
        while (*str) {
            _base_putc(*str++);
        }
        return 0;
    }

    int RawSerial::printf(const char* format, ...)
    {
        va_list arg;
        va_start(arg, format);
        int n = rsnprintf(this, STRING_STACK_LIMIT, format, arg);
        if (n >= STRING_STACK_LIMIT) {
            n = rsnprintf(this, n + 1, format, arg);
        }
        va_end(arg);
        return n;
    }

}

#endif
