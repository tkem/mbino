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
#ifndef MBINO_INTERRUPT_IN_H
#define MBINO_INTERRUPT_IN_H

#include "platform/platform.h"

#if defined (DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)

#include "hal/gpio_api.h"
#include "hal/gpio_irq_api.h"
#include "platform/Callback.h"
#include "platform/NonCopyable.h"

namespace mbino {

    class InterruptIn : private NonCopyable<InterruptIn> {
        Callback<void()> _rise;
        Callback<void()> _fall;

    public:

        InterruptIn(PinName pin);

        // mbino restriction: destructor not virtual
        ~InterruptIn() {
            gpio_irq_free(&gpio_irq);
        }

        int read() {
            return gpio_read(&gpio);
        }

        operator int() {
            return read();
        }

        // mbino extension: pass const reference to func
        void rise(const Callback<void()>& func);

        // mbino extension: pass const reference to func
        void fall(const Callback<void()>& func);

        void mode(PinMode pull) {
            gpio_mode(&gpio, pull);
        }

        void enable_irq() {
            gpio_irq_enable(&gpio_irq);
        }

        void disable_irq() {
            gpio_irq_disable(&gpio_irq);
        }

    protected:
        gpio_t gpio;
        gpio_irq_t gpio_irq;

    private:
        // mbino extension: change id type to intptr_t
        static void _irq_handler(intptr_t id, gpio_irq_event event);
    };

}

#endif

#endif
