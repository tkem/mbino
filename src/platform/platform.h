/* mbino - mbed APIs for the Arduino platform
 * Copyright (c) 2018 Thomas Kemmer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_PLATFORM_H
#define MBED_PLATFORM_H

#include <mbed_config.h>

// neither AVR not SAMD seem to provide <cstddef> et al.
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __WITH_AVRLIBC__
int vsscanf(const char *s, const char *fmt, va_list ap);
#endif

#if __cplusplus
// these must not be defined as macros
#ifdef getc
#undef getc
#endif
#ifdef putc
#undef putc
#endif
// used in FileBase.cpp
namespace std {
    using ::strncmp;
    using ::strlen;
}
#endif

#include "platform/mbed_retarget.h"
#include "platform/mbed_toolchain.h"
#include "device.h"

#if __cplusplus
#include "PlatformInit.h"
#endif

#endif
