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

#include <limits.h>

#include "hal/us_ticker_api.h"

static uint32_t next_interrupt = (uint32_t)INT32_MAX;

static const ticker_info_t ticker_info = {
    .frequency = 1000000,
    .bits = 32
};

void us_ticker_init(void)
{
}

uint32_t us_ticker_read(void)
{
    return micros();
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    next_interrupt = timestamp;
}
void us_ticker_disable_interrupt(void)
{
    // TODO
}

void us_ticker_clear_interrupt(void)
{
    // nothing to do?
}

void us_ticker_fire_interrupt(void)
{
    next_interrupt = us_ticker_read();  // TODO: trigger manually?
}

const ticker_info_t* us_ticker_get_info(void)
{
    return &ticker_info;
}

int sysTickHook(void)
{
    if ((int32_t)(us_ticker_read() - next_interrupt) >= 0) {
        us_ticker_irq_handler();
    }
    return 0;
}

#endif
