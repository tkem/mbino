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
#ifndef MBINO_PWMOUT_API_H
#define MBINO_PWMOUT_API_H

#include "device.h"

#if DEVICE_PWMOUT

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pwmout_s pwmout_t;

void pwmout_init(pwmout_t *obj, PinName pin);

void pwmout_free(pwmout_t *obj);

// mbino extension
void pwmout_write_u16(pwmout_t *obj, uint16_t value);

// mbino extension: inline floating point
static inline void pwmout_write(pwmout_t *obj, float value)
{
    if (value <= 0.0f) {
        pwmout_write_u16(obj, 0);
    } else if (value >= 1.0f) {
        pwmout_write_u16(obj, 0xffff);
    } else {
        pwmout_write_u16(obj, value * 65535.0f);
    }
}

// mbino extension
uint16_t pwmout_read_u16(pwmout_t *obj);

// mbino extension: inline floating point
static inline float pwmout_read(pwmout_t *obj)
{
    return pwmout_read_u16(obj) * (1.0f / 65535.0f);
}

// mbino extension: change us type to long
void pwmout_period_us(pwmout_t *obj, long us);

// mbino extension: change ms type to long, inline forward
static inline void pwmout_period_ms(pwmout_t* obj, long ms)
{
    pwmout_period_us(obj, ms * 1000);
}

// mbino extension: inline floating point
static inline void pwmout_period(pwmout_t* obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

// mbino extension: change us type to long
void pwmout_pulsewidth_us(pwmout_t* obj, long us);

// mbino extension: change ms type to long, inline forward
static inline void pwmout_pulsewidth_ms(pwmout_t* obj, long ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

// mbino extension: inline floating point
static inline void pwmout_pulsewidth(pwmout_t* obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

#ifdef __cplusplus
}
#endif

#endif

#endif
