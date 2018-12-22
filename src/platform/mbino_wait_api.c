/* mbino - mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
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
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <Arduino.h>

#include <limits.h>

#include "mbed_wait_api.h"

void wait(float s) {
    if (s > INT_MAX * 0.000001f) {
        wait_ms(s * 1000.0f);
    } else {
        wait_us(s * 1000000.0f);
    }
}

void wait_ms(int ms)
{
    // delay() requires interrupts to be enabled (at least on AVR)
    while (ms--) {
        delayMicroseconds(1000);
    }
}

void wait_us(int us)
{
    // "Currently, the largest value that will produce an accurate
    // delay is 16383".
    // https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
    switch ((unsigned)us >> 14) {
    case 3:
        delayMicroseconds(0x3fff);
        // fallthrough
    case 2:
        delayMicroseconds(0x3fff);
        // fallthrough
    case 1:
        delayMicroseconds(0x3fff);
        // fallthrough
    case 0:
        delayMicroseconds(us & 0x3fff);
    }
}
