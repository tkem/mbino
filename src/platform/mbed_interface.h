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
#ifndef MBINO_MBED_INTERFACE_H
#define MBINO_MBED_INTERFACE_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_SEMIHOST
#error "Semihosting not supported"
#endif

void mbed_mac_address(char *mac);

void mbed_die(void);

void mbed_error_printf(const char* format, ...);

void mbed_error_vfprintf(const char *format, va_list arg);

// mbino extension
void mbed_error_puts(const char* message);

#ifdef __cplusplus
}
#endif

#endif
