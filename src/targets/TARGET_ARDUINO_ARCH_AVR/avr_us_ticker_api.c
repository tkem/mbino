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

#include <limits.h>

#include "hal/us_ticker_api.h"

#ifdef MBINO_US_TICKER_STATISTICS

#include <stdio.h>

typedef struct {
    unsigned min;
    unsigned max;
} stats_time_t;

static struct {
    unsigned num_fired;
    stats_time_t delay;
    stats_time_t  duration;
    stats_time_t total;
} stats = {
    0, { UINT_MAX, 0 }, { UINT_MAX, 0 }, { UINT_MAX, 0 }
};

static void update_time(stats_time_t *p, unsigned d)
{
    if (p->min > d) {
        p->min = d;
    }
    if (p->max < d) {
        p->max = d;
    }
}

static void print_time(const char *msg, stats_time_t *p)
{
    printf_P(PSTR("%S: %u/%u us\n"), msg, p->min, p->max);
}

void us_ticker_print_statistics(void)
{
    printf_P(PSTR("Ticker interrupt fired: %u\n"), stats.num_fired);
    print_time(PSTR("Ticker IRQ handler delay"), &stats.delay);
    print_time(PSTR("Ticker IRQ handler duration"), &stats.duration);
    print_time(PSTR("Ticker IRQ duration"), &stats.total);
}

#else

void us_ticker_print_statistics(void) {}

#endif

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

void us_ticker_free(void)
{
    // TODO: disable Timer0?
    us_ticker_disable_interrupt();
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
    // TODO: software interrupt?
    uint8_t sreg = SREG;
    cli();
    enable_interrupt();
    fired_interrupt = true;
#ifdef MBINO_US_TICKER_STATISTICS
    next_interrupt = micros();
    ++stats.num_fired;
#endif
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

#ifdef MBINO_US_TICKER_STATISTICS

SIGNAL(TIMER0_COMPA_vect)
{
    uint32_t t1 = micros();
    if (fired_interrupt || (int32_t)(t1 - next_interrupt) >= 0) {
        do {
            uint32_t t2 = micros();
            update_time(&stats.delay, t2 - next_interrupt);
            fired_interrupt = false;
            us_ticker_irq_handler();
            update_time(&stats.duration, micros() - t2);
        } while (fired_interrupt);
    }
    update_time(&stats.total, micros() - t1);
}

#else

SIGNAL(TIMER0_COMPA_vect)
{
    if (fired_interrupt || (int32_t)(micros() - next_interrupt) >= 0) {
        do {
            fired_interrupt = false;
            us_ticker_irq_handler();
        } while (fired_interrupt);
    }
}

#endif

#endif
