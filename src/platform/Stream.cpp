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
#include "Stream.h"

namespace mbino {

    Stream::Stream()
    {
        _file = fdopen(this, "w+");
    }

    Stream::~Stream()
    {
        fclose(_file);
    }

    int Stream::printf(const char* format, ...)
    {
        va_list arg;
        va_start(arg, format);
        int r = vfprintf(_file, format, arg);
        va_end(arg);
        return r;
    }

    int Stream::scanf(const char* format, ...)
    {
        va_list arg;
        va_start(arg, format);
        int r = vfscanf(_file, format, arg);
        va_end(arg);
        return r;
    }

    int Stream::vprintf(const char* format, va_list args)
    {
        return vfprintf(_file, format, args);
    }

    int Stream::vscanf(const char* format, va_list args)
    {
        return vfscanf(_file, format, args);
    }

}
