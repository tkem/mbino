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

#include "hal/us_ticker_api.h"

#include <Arduino.h>

static uint32_t next_interrupt = MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA;

void us_ticker_init(void)
{
    uint8_t sreg = SREG;
    cli();
    // Timer0 is already used for millis() - we'll just interrupt
    // somewhere in the middle (as compare register may be set by
    // PWM output) and call the COMPA function below
#if defined(TIMSK) && defined(TOIE0)
    TIMSK |= _BV(OCIE0A);
#elif defined(TIMSK0) && defined(TOIE0)
    TIMSK0 |= _BV(OCIE0A);
#else
#error Cannot set Timer 0 compare interrupt
#endif
    SREG = sreg;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint8_t sreg = SREG;
    cli();
    next_interrupt = timestamp;
    SREG = sreg;
}

SIGNAL(TIMER0_COMPA_vect)
{
    if ((int32_t)(us_ticker_read() - next_interrupt) >= 0) {
        us_ticker_irq_handler();
    }
}

#endif
