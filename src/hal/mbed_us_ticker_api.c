/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
 *
 * mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "us_ticker_api.h"

static ticker_event_queue_t us_ticker_events;

static const ticker_interface_t us_ticker_interface = {
    .init = us_ticker_init,
    .read = us_ticker_read,
    .set_interrupt = us_ticker_set_interrupt
};

static const ticker_data_t us_ticker_data = {
    .interface = &us_ticker_interface,
    .queue = &us_ticker_events
};

const ticker_data_t *get_us_ticker_data(void)
{
    return &us_ticker_data;
}

void us_ticker_irq_handler(void)
{
    ticker_irq_handler(&us_ticker_data);
}
