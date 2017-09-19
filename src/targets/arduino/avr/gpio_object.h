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
#ifndef MBINO_GPIO_OBJECT_H
#define MBINO_GPIO_OBJECT_H

#ifdef __cplusplus
extern "C"{
#endif

typedef struct {
    volatile uint8_t *reg;
    uint8_t port;
    uint8_t mask;
} gpio_t;

static inline int gpio_read(gpio_t *obj)
{
    return (*obj->reg & obj->mask) ? 1 : 0;
}

static inline int gpio_is_connected(const gpio_t *obj)
{
    return obj->port != NC;
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
