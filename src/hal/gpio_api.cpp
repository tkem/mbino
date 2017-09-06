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
#include "gpio_api.h"
#include "timers.h"

#include <Arduino.h>

namespace mbino {

    static void gpio_pwm_off(PinName pin)
    {
        uint8_t timer = digitalPinToTimer(pin);
        if (timer != NOT_ON_TIMER) {
            volatile uint8_t* tccr = timerToControlRegister(timer);
            uint8_t com = timerToCompareOutputModeMask(timer);
            uint8_t sreg = SREG;
            cli();
            *tccr &= ~com;
            SREG = sreg;
        }
    }

    void gpio_init_in(gpio_t* obj, PinName pin, PinMode pull)
    {
        uint8_t port = digitalPinToPort(pin);
        uint8_t mask = digitalPinToBitMask(pin);
        port_init_in(obj, port, mask, pull);
        gpio_pwm_off(pin);  // in case it's later turned into an output
    }

    void gpio_init_out(gpio_t* obj, PinName pin, bool value)
    {
        uint8_t port = digitalPinToPort(pin);
        uint8_t mask = digitalPinToBitMask(pin);
        port_init_out(obj, port, mask, value ? mask : 0);
        gpio_pwm_off(pin);
    }

    void gpio_write(gpio_t* obj, bool value)
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
}
