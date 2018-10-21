/* mbino - mbed APIs for the Arduino platform
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
#ifndef MBINO_AVR_GPIO_OBJECT_H
#define MBINO_AVR_GPIO_OBJECT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    volatile uint8_t *reg;
    unsigned dir : 1;
    unsigned mode : 1;
    unsigned value : 1;
    unsigned reserved : 1;
    unsigned port : 4;
    unsigned mask : 8;
} gpio_t;

static inline int gpio_read(gpio_t *obj)
{
    return (*obj->reg & obj->mask) ? 1 : 0;
}

static inline int gpio_is_connected(const gpio_t *obj)
{
    return obj->mask != 0;
}

#ifdef __cplusplus
}
#endif

#endif
