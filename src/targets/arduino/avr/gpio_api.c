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

#include "hal/gpio_api.h"
#include "hal/port_api.h"
#include "timers.h"

#include <Arduino.h>

static void gpio_pwm_off(PinName pin)
{
    uint8_t timer = digitalPinToTimer(pin);
    if (timer != NOT_ON_TIMER) {
        volatile uint8_t *tccr = timerToControlRegister(timer);
        uint8_t com = timerToCompareOutputModeMask(timer);
        uint8_t sreg = SREG;
        cli();
        *tccr &= ~com;
        SREG = sreg;
    }
}

void gpio_init_in_ex(gpio_t *obj, PinName pin, PinMode mode)
{
    uint8_t port = digitalPinToPort(pin);
    uint8_t mask = digitalPinToBitMask(pin);
    port_init_in_ex(obj, port, mask, mode);
    gpio_pwm_off(pin);  // in case it's later turned into an output
}

void gpio_init_out_ex(gpio_t *obj, PinName pin, int value)
{
    uint8_t port = digitalPinToPort(pin);
    uint8_t mask = digitalPinToBitMask(pin);
    port_init_out_ex(obj, port, mask, value ? mask : 0);
    gpio_pwm_off(pin);
}

void gpio_dir_in(gpio_t *obj, PinMode mode)
{
    port_dir_in(obj, mode);
}

void gpio_dir_out(gpio_t *obj, int value)
{
    port_dir_out(obj, value ? obj->mask : 0);
}

int gpio_read(gpio_t *obj)
{
    return !!port_read(obj);
}

void gpio_write(gpio_t *obj, int value)
{
    uint8_t sreg = SREG;
    cli();
    if (value) {
        *obj->reg |= obj->mask;
    } else {
        *obj->reg &= ~obj->mask;
    }
    SREG = sreg;
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    port_mode(obj, mode);
}

int gpio_is_connected(const gpio_t *obj)
{
    return obj->port != NC;
}

#endif
