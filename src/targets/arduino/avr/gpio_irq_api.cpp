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
#ifdef ARDUINO_ARCH_AVR

#include "hal/gpio_irq_api.h"

#include <Arduino.h>
#include <wiring_private.h>

// see e.g. https://github.com/damellis/attiny/issues/81
#ifndef digitalPinToInterrupt
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
#define digitalPinToInterrupt(p) ((p) == 8 ? 0 : NOT_AN_INTERRUPT)
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#define digitalPinToInterrupt(p) ((p) == 2 ? 0 : NOT_AN_INTERRUPT)
#else
#error digitalPinToInterrupt() not available for this platform
#endif
#endif

typedef void(*irq_handler)();

static gpio_irq_t* gpio_irq_objects[EXTERNAL_NUM_INTERRUPTS];

template<int N>
static void gpio_irq_isr()
{
    gpio_irq_t* obj = gpio_irq_objects[N];
    // try to detect event as early as possible in irq handler
    gpio_irq_event event = (*obj->reg & obj->mask) ? IRQ_RISE : IRQ_FALL;
    obj->handler(obj->id, event);
}

// TODO: PROGMEM?
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

static void gpio_irq_attach(gpio_irq_t* obj)
{
    static const uint8_t mode[] = {0, RISING, FALLING, CHANGE};
    if (obj->enabled && obj->events) {
        attachInterrupt(obj->irq, gpio_irq_handlers[obj->irq], mode[obj->events]);
    } else {
        detachInterrupt(obj->irq);
    }
}

int gpio_irq_init(gpio_irq_t* obj, PinName pin, gpio_irq_handler handler, intptr_t id)
{
    int irq = digitalPinToInterrupt(pin);
    if (irq != NOT_AN_INTERRUPT) {
        obj->handler = handler;
        obj->id = id;
        obj->reg = portInputRegister(digitalPinToPort(pin));
        obj->mask = digitalPinToBitMask(pin);
        obj->irq = irq;
        obj->events = IRQ_NONE;
        obj->enabled = true;
        gpio_irq_objects[irq] = obj;
        return 0;
    } else {
        return -1;
    }
}

void gpio_irq_free(gpio_irq_t* obj)
{
    if (obj->events) {
        detachInterrupt(obj->irq);
    }
    gpio_irq_objects[obj->irq] = 0;
}

void gpio_irq_set(gpio_irq_t* obj, gpio_irq_event event, int enable)
{
    if (enable) {
        obj->events |= event;
    } else {
        obj->events &= ~event;
    }
    gpio_irq_attach(obj);
}

void gpio_irq_enable(gpio_irq_t* obj)
{
    obj->enabled = true;
    gpio_irq_attach(obj);
}

void gpio_irq_disable(gpio_irq_t* obj)
{
    obj->enabled = false;
    detachInterrupt(obj->irq);
}

#endif
