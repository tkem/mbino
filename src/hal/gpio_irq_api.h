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
#ifndef MBINO_GPIO_IRQ_API_H
#define MBINO_GPIO_IRQ_API_H

#include "platform/platform.h"

#include <stdint.h>

namespace mbino {

    enum gpio_irq_event { IRQ_NONE = 0, IRQ_RISE = 1, IRQ_FALL = 2 };

    typedef void(*gpio_irq_handler)(intptr_t id, gpio_irq_event event);

    struct gpio_irq_t {
        gpio_irq_handler handler;
        intptr_t id;
        const volatile uint8_t* reg;
        uint8_t mask;
        uint8_t irq;
        uint8_t events;
        bool enabled;
    };

    int gpio_irq_init(gpio_irq_t* obj, PinName pin, gpio_irq_handler handler, intptr_t id);

    void gpio_irq_free(gpio_irq_t* obj);

    void gpio_irq_set(gpio_irq_t* obj, gpio_irq_event event, bool enable);

    void gpio_irq_enable(gpio_irq_t* obj);

    void gpio_irq_disable(gpio_irq_t* obj);

}

#endif
