/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
 *
 * mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.  See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include "InterruptIn.h"

#include <Arduino.h>

namespace mbino {

    static void donothing() {}

    InterruptIn::InterruptIn(PinName pin) : _rise(donothing), _fall(donothing)
    {
        gpio_irq_init(&gpio_irq, pin, &InterruptIn::_irq_handler, (intptr_t)this);
        gpio_init_in(&gpio, pin);
    }

    InterruptIn::~InterruptIn()
    {
        gpio_irq_free(&gpio_irq);
    }

    void InterruptIn::rise(const Callback<void()>& func)
    {
        uint8_t sreg = SREG;
        cli();
        if (func) {
            _rise = func;
            gpio_irq_set(&gpio_irq, IRQ_RISE, true);
        } else {
            _rise = donothing;
            gpio_irq_set(&gpio_irq, IRQ_RISE, false);
        }
        SREG = sreg;
    }

    void InterruptIn::fall(const Callback<void()>& func) {
        uint8_t sreg = SREG;
        cli();
        if (func) {
            _fall = func;
            gpio_irq_set(&gpio_irq, IRQ_FALL, true);
        } else {
            _fall = donothing;
            gpio_irq_set(&gpio_irq, IRQ_FALL, false);
        }
        SREG = sreg;
    }

    void InterruptIn::_irq_handler(intptr_t id)
    {
        InterruptIn* obj = (InterruptIn*)id;
        if (*obj) {
            obj->_rise();
        } else {
            obj->_fall();
        }
    }
}
