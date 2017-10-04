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
#include "mbed_error.h"
#include "mbed_interface.h"
#include "mbed_toolchain.h"

#include <stdarg.h>
#include <stdbool.h>

static bool error_in_progress = false;

MBED_WEAK void error(const char *format, ...)
{
    if (!error_in_progress) {
        error_in_progress = true;
#ifndef NDEBUG
        va_list arg;
        va_start(arg, format);
        mbed_error_vfprintf(format, arg);
        va_end(arg);
#endif
        // mbino extension: call mbed_die() directly instead of exit(1)
        mbed_die();
    }
}
