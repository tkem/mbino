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

#include <Arduino.h>

#include <limits.h>

#include "hal/us_ticker_api.h"

static uint32_t next_interrupt = (uint32_t)INT32_MAX;

static bool fired_interrupt = false;

static void enable_interrupt(void)
{
#if defined(TIMSK) && defined(TOIE0)
    TIMSK |= (1 << OCIE0A);
#elif defined(TIMSK0) && defined(TOIE0)
    TIMSK0 |= (1 << OCIE0A);
#else
#error Cannot set Timer 0 compare interrupt
#endif
}

static void disable_interrupt(void)
{
#if defined(TIMSK) && defined(TOIE0)
    TIMSK &= ~(1 << OCIE0A);
#elif defined(TIMSK0) && defined(TOIE0)
    TIMSK0 &= ~(1 << OCIE0A);
#else
#error Cannot clear Timer 0 compare interrupt
#endif
}

void us_ticker_init(void)
{
    // Timer0 is already initialized by Arduino's init(), and is
    // already used for micros() and millis() - we'll just interrupt
    // somewhere in the middle (as compare register may be set by PWM
    // output) and call the COMPA function below...
}

uint32_t us_ticker_read(void)
{
    return micros();
}

void us_ticker_disable_interrupt(void)
{
    uint8_t sreg = SREG;
    cli();
    disable_interrupt();
    SREG = sreg;
}

void us_ticker_clear_interrupt(void)
{
    // nothing to do here
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint8_t sreg = SREG;
    cli();
    enable_interrupt();
    next_interrupt = timestamp;
    SREG = sreg;
}

void us_ticker_fire_interrupt(void)
{
    uint8_t sreg = SREG;
    cli();
    enable_interrupt();
    fired_interrupt = true;
    SREG = sreg;
}

const ticker_info_t* us_ticker_get_info(void)
{
    static const ticker_info_t ticker_info = {
        .frequency = 1000000,
        .bits = 32
    };
    return &ticker_info;
}

SIGNAL(TIMER0_COMPA_vect)
{
    // TODO: profile!
    while (fired_interrupt || (int32_t)(micros() - next_interrupt) >= 0) {
        fired_interrupt = false;
        us_ticker_irq_handler();
    }
}

#endif
