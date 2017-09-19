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
#ifndef MBINO_ANALOGIN_API_H
#define MBINO_ANALOGIN_API_H

#include "device.h"

#if DEVICE_ANALOGIN

#ifdef __cplusplus
extern "C" {
#endif

typedef struct analogin_s analogin_t;

void analogin_init(analogin_t *obj, PinName pin);

uint16_t analogin_read_u16(analogin_t *obj);

// mbino extension: inline floating point
static inline float analogin_read(analogin_t *obj)
{
    return analogin_read_u16(obj) * (1.0f / 65535.0f);
}

#ifdef __cplusplus
}
#endif

#endif

#endif
