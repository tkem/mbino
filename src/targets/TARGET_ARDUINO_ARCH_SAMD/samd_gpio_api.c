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
#ifdef ARDUINO_ARCH_SAMD

#include <Arduino.h>

#include "hal/gpio_api.h"

static uint32_t nc = 0;  // dummy register

static void gpio_dir_in(gpio_t *obj, PinMode mode)
{
    if (gpio_is_connected(obj)) {
        uint32_t primask = __get_PRIMASK();
        __disable_irq();
        switch (mode) {
        case PullNone:
            PORT->Group[obj->port].PINCFG[obj->pin].reg = PORT_PINCFG_INEN;
            PORT->Group[obj->port].DIRCLR.reg = 1 << obj->pin;
            break;
        case PullUp:
            PORT->Group[obj->port].PINCFG[obj->pin].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
            PORT->Group[obj->port].DIRCLR.reg = 1 << obj->pin;
            PORT->Group[obj->port].OUTSET.reg = 1 << obj->pin;
            break;
        case PullDown:
            PORT->Group[obj->port].PINCFG[obj->pin].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
            PORT->Group[obj->port].DIRCLR.reg = 1 << obj->pin;
            PORT->Group[obj->port].OUTCLR.reg = 1 << obj->pin;
            break;
        }
        obj->data = &PORT->Group[obj->port].IN.reg;  // TODO: init only?
        obj->outset = &nc;  // FIXME
        obj->outclr = &nc;  // FIXME
        __set_PRIMASK(primask);
    }
}

static void gpio_dir_out(gpio_t *obj, int value)
{
    if (gpio_is_connected(obj)) {
        uint32_t primask = __get_PRIMASK();
        __disable_irq();
        PORT->Group[obj->port].PINCFG[obj->pin].reg = PORT_PINCFG_INEN;
        if (value) {
            PORT->Group[obj->port].OUTSET.reg = 1 << obj->pin;
        } else {
            PORT->Group[obj->port].OUTCLR.reg = 1 << obj->pin;
        }
        PORT->Group[obj->port].DIRSET.reg = 1 << obj->pin;
        obj->data = &PORT->Group[obj->port].IN.reg;  // TODO: init only, re-check
        obj->outset = &PORT->Group[obj->port].OUTSET.reg;
        obj->outclr = &PORT->Group[obj->port].OUTCLR.reg;
        __set_PRIMASK(primask);
    }
}

void gpio_init(gpio_t *obj, PinName pin)
{
    if (pin != NC) {
        obj->port = g_APinDescription[pin].ulPort;
        obj->pin = g_APinDescription[pin].ulPin;
        // TODO: dir, regs - or set to nc?
    } else {
        obj->port = PortA;  // FIXME
        obj->pin = ~0U;     // FIXME
        obj->data = &nc;
        obj->outset = &nc;
        obj->outclr = &nc;
    }
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    if (gpio_is_connected(obj)) {
        uint32_t primask = __get_PRIMASK();
        __disable_irq();
        switch (mode) {
        case PullNone:
            PORT->Group[obj->port].PINCFG[obj->pin].bit.PULLEN = 0;
            break;
        case PullUp:
            PORT->Group[obj->port].PINCFG[obj->pin].bit.PULLEN = 1;
            PORT->Group[obj->port].OUTSET.reg = 1 << obj->pin;
            break;
        case PullDown:
            PORT->Group[obj->port].PINCFG[obj->pin].bit.PULLEN = 1;
            PORT->Group[obj->port].OUTCLR.reg = 1 << obj->pin;
            break;
        }
        __set_PRIMASK(primask);
    }
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    if (direction == PIN_INPUT) {
        gpio_dir_in(obj, PullDefault);  // FIXME
    } else {
        gpio_dir_out(obj, 0);  // FIXME
    }
}

#endif
