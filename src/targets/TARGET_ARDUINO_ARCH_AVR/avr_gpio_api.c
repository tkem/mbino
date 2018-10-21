/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
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

#include <Arduino.h>

#include "hal/gpio_api.h"

#include "avr_timers.h"

static void gpio_dir_in(gpio_t *obj)
{
    volatile uint8_t *mode = portModeRegister(obj->port);
    volatile uint8_t *input = portInputRegister(obj->port);
    volatile uint8_t *output = portOutputRegister(obj->port);

    obj->dir = PIN_INPUT;
    // save value if currently set as output
    if (*mode & obj->mask) {
        obj->value = (*output & obj->mask) ? 1 : 0;
    }
    obj->reg = input;

    uint8_t sreg = SREG;
    cli();
    *mode &= ~obj->mask;
    if (obj->mode == PullUp) {
        *output |= obj->mask;
    } else {
        *output &= obj->mask;
    }
    SREG = sreg;
}

static void gpio_dir_out(gpio_t *obj)
{
    volatile uint8_t *mode = portModeRegister(obj->port);
    volatile uint8_t *output = portOutputRegister(obj->port);

    obj->dir = PIN_OUTPUT;
    // save mode if currently set as input
    if (!(*mode & obj->mask)) {
        obj->mode = (*output & obj->mask) ? PullUp : PullNone;
    }
    obj->reg = output;

    uint8_t sreg = SREG;
    cli();
    if (obj->value) {
        *output |= obj->mask;
    } else {
        *output &= ~obj->mask;
    }
    *mode |= obj->mask;
    SREG = sreg;
}

uint32_t gpio_set(PinName pin)
{
    // TODO: disable analog in?
    // disable PWM for this pin
    uint8_t timer = digitalPinToTimer(pin);
    if (timer != NOT_ON_TIMER) {
        volatile uint8_t *tccr = timerToControlRegister(timer);
        uint8_t com = timerToCompareOutputModeMask(timer);
        uint8_t sreg = SREG;
        cli();
        *tccr &= ~com;
        SREG = sreg;
    }
    return digitalPinToBitMask(pin);
}

void gpio_init(gpio_t *obj, PinName pin)
{
    static uint8_t ncreg = 0;  // dummy register

    if (pin != NC) {
        obj->port = digitalPinToPort(pin);
        obj->mask = gpio_set(pin);
        if (*portModeRegister(obj->port) & obj->mask) {
            obj->reg = portOutputRegister(obj->port);
            obj->dir = PIN_OUTPUT;
        } else {
            obj->reg = portInputRegister(obj->port);
            obj->dir = PIN_INPUT;
        }
        obj->mode = PullDefault;
        obj->value = 0;
    } else {
        obj->reg = &ncreg;
        obj->port = 0;
        obj->mask = 0;
    }
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    // driver code locks this
    if (gpio_is_connected(obj)) {
        if (obj->dir == PIN_INPUT) {
            volatile uint8_t *output = portOutputRegister(obj->port);
            uint8_t sreg = SREG;
            cli();
            if (mode == PullUp) {
                *output |= obj->mask;
            } else {
                *output &= obj->mask;
            }
            SREG = sreg;
        } else {
            obj->mode = mode;
        }
    }
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    // driver code locks this
    if (gpio_is_connected(obj)) {
        if (direction == PIN_INPUT) {
            gpio_dir_in(obj);
        } else {
            gpio_dir_out(obj);
        }
    }
}

void gpio_write(gpio_t *obj, int value)
{
    // driver code assumes gpio_write() is thread- and interrupt-safe
    uint8_t sreg = SREG;
    cli();
    if (obj->dir == PIN_OUTPUT) {
        if (value) {
            *obj->reg |= obj->mask;
        } else {
            *obj->reg &= ~obj->mask;
        }
    } else {
        obj->value = value ? 1 : 0;
    }
    SREG = sreg;
}

#endif
