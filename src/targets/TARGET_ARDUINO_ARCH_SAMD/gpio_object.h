/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
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
#ifndef MBINO_SAMD_GPIO_OBJECT_H
#define MBINO_SAMD_GPIO_OBJECT_H

#include <stdint.h>

#include "PortNames.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct {
    PortName port;
    uint32_t pin;
    volatile const uint32_t *data;
    volatile uint32_t *outset;
    volatile uint32_t *outclr;
} gpio_t;

static inline int gpio_read(gpio_t *obj)
{
    return (*obj->data & (1 << obj->pin)) ? 1 : 0;
}

static inline void gpio_write(gpio_t *obj, int value)
{
    *(value ? obj->outset : obj->outclr) = (1 << obj->pin);
}

static inline int gpio_is_connected(const gpio_t *obj)
{
    return obj->pin != ~0U;
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
