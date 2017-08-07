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

#include <stdio.h>
#include <stdarg.h>

#define STRING_STACK_LIMIT 80

namespace mbino {
    int RawSerial::printf(const char *format, ...) {
        va_list arg;
        va_start(arg, format);
        char dummy_buf[1];
        int len = vsnprintf(dummy_buf, sizeof(dummy_buf), format, arg);
        if (len < STRING_STACK_LIMIT) {
            char temp[STRING_STACK_LIMIT];
            vsprintf(temp, format, arg);
            puts(temp);
        } else {
            char *temp = new char[len + 1];
            vsprintf(temp, format, arg);
            puts(temp);
            delete[] temp;
        }
        va_end(arg);
        return len;
    }
}
