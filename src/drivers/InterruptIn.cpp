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

#ifdef DEVICE_INTERRUPTIN

#include "platform/mbed_critical.h"

namespace mbino {

    static void donothing() {}

    InterruptIn::InterruptIn(PinName pin) : _rise(donothing), _fall(donothing)
    {
        gpio_irq_init(&gpio_irq, pin, &InterruptIn::_irq_handler, (intptr_t)this);
        gpio_init_in(&gpio, pin);
    }

    void InterruptIn::rise(const Callback<void()>& func)
    {
        core_util_critical_section_enter();
        if (func) {
            _rise = func;
            gpio_irq_set(&gpio_irq, IRQ_RISE, true);
        } else {
            _rise = donothing;
            gpio_irq_set(&gpio_irq, IRQ_RISE, false);
        }
        core_util_critical_section_exit();
    }

    void InterruptIn::fall(const Callback<void()>& func) {
        core_util_critical_section_enter();
        if (func) {
            _fall = func;
            gpio_irq_set(&gpio_irq, IRQ_FALL, true);
        } else {
            _fall = donothing;
            gpio_irq_set(&gpio_irq, IRQ_FALL, false);
        }
        core_util_critical_section_exit();
    }

    void InterruptIn::_irq_handler(intptr_t id, gpio_irq_event event)
    {
        InterruptIn* obj = reinterpret_cast<InterruptIn*>(id);
        switch (event) {
        case IRQ_RISE:
            obj->_rise();
            break;
        case IRQ_FALL:
            obj->_fall();
            break;
        case IRQ_NONE:
            break;
        }
    }

}

#endif
