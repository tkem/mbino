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

// TODO: device.h forward in platform dir?
#include "hal/device.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_SEMIHOST
#error "Semihosting not supported"
#endif

static inline void mbed_mac_address(char *mac) {
    mac[0] = 0x00;
    mac[1] = 0x02;
    mac[2] = 0xF7;
    mac[3] = 0xF0;
    mac[4] = 0x00;
    mac[5] = 0x00;
}

void mbed_die(void);

// mbino extension
void mbed_error_puts(const char *message);

void mbed_error_printf(const char *format, ...);

void mbed_error_vfprintf(const char *format, va_list arg);

#ifdef __cplusplus
}
#endif

#endif
