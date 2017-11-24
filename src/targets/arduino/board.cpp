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
#include "hal/gpio_api.h"
#include "platform/mbed_interface.h"

#include <Arduino.h>

static void delay_ms(unsigned ms)
{
    while (ms--) {
        delayMicroseconds(1000);
    }
}

void mbed_die(void)
{
#ifdef USBCON
    interrupts();
#else
    noInterrupts();
#endif
    gpio_t led;
    gpio_init_out(&led, LED1);

    for (;;) {
        for (int i = 0; i != 4; ++i) {
            gpio_write(&led, 1);
            delay_ms(150);
            gpio_write(&led, 0);
            delay_ms(150);
        }
        for (int i = 0; i != 4; ++i) {
            gpio_write(&led, 1);
            delay_ms(400);
            gpio_write(&led, 0);
            delay_ms(400);
        }
    }
}
