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
#include "gpio_irq_api.h"

#include <Arduino.h>
#include <wiring_private.h>

namespace mbino {
    typedef void(*irq_handler)();

    static gpio_irq_t* gpio_irq_objects[EXTERNAL_NUM_INTERRUPTS];

    template<int N>
    void gpio_irq_isr()
    {
        gpio_irq_t* obj = gpio_irq_objects[N];
        obj->handler(obj->id);
    }

    static const irq_handler gpio_irq_handlers[EXTERNAL_NUM_INTERRUPTS] = {
#if EXTERNAL_NUM_INTERRUPTS > 0
        gpio_irq_isr<0>,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 1
        gpio_irq_isr<1>,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 2
        gpio_irq_isr<2>,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 3
        gpio_irq_isr<3>,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 4
        gpio_irq_isr<4>,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 5
        gpio_irq_isr<5>,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 6
        gpio_irq_isr<6>,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 7
        gpio_irq_isr<7>,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 8
#warning There are more than 8 external interrupts. Some callbacks may not be initialized.
#endif
    };

    int gpio_irq_init(gpio_irq_t* obj, PinName pin, gpio_irq_handler handler, intptr_t id)
    {
        uint8_t irq = obj->irq = digitalPinToInterrupt(pin);
        obj->handler = handler;
        obj->id = id;
        // TODO: error handling
        gpio_irq_objects[irq] = obj;
        return 0;
    }

    void gpio_irq_free(gpio_irq_t* obj)
    {
        if (obj->events) {
            detachInterrupt(obj->irq);
        }
        gpio_irq_objects[obj->irq] = 0;
    }

    void gpio_irq_set(gpio_irq_t* obj, gpio_irq_event event, bool enable)
    {
        if (enable) {
            obj->events |= event;
        } else {
            obj->events &= ~event;
        }
        switch (obj->events) {
        case IRQ_RISE | IRQ_FALL:
            attachInterrupt(obj->irq, gpio_irq_handlers[obj->irq], CHANGE);
            break;
        case IRQ_FALL:
            attachInterrupt(obj->irq, gpio_irq_handlers[obj->irq], FALLING);
            break;
        case IRQ_RISE:
            attachInterrupt(obj->irq, gpio_irq_handlers[obj->irq], RISING);
            break;
        default:
            detachInterrupt(obj->irq);
        }
    }

    void gpio_irq_enable(gpio_irq_t* obj)
    {
        switch (obj->events) {
        case IRQ_RISE | IRQ_FALL:
            attachInterrupt(obj->irq, gpio_irq_handlers[obj->irq], CHANGE);
            break;
        case IRQ_FALL:
            attachInterrupt(obj->irq, gpio_irq_handlers[obj->irq], FALLING);
            break;
        case IRQ_RISE:
            attachInterrupt(obj->irq, gpio_irq_handlers[obj->irq], RISING);
            break;
        }
    }

    void gpio_irq_disable(gpio_irq_t* obj)
    {
        detachInterrupt(obj->irq);
    }

}
