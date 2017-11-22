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
#include "hal/rtc_api.h"
#include "hal/ticker_api.h"
#include "hal/us_ticker_api.h"

#include <stdbool.h>
#include <stddef.h>

// AVR Libc uses a time offset from Midnight Jan 1 2000.
#ifndef UNIX_OFFSET
#define UNIX_OFFSET 0
#endif

static ticker_data_t const* rtc_ticker = NULL;
static time_t rtc_offset = 0;

static time_t rtc_ticker_read(void)
{
    return ticker_read_us(rtc_ticker) / 1000000;
}

void rtc_init(void)
{
    rtc_ticker = get_us_ticker_data();
}

void rtc_free(void)
{
    rtc_ticker = NULL;
}

int rtc_isenabled(void)
{
    return rtc_ticker != NULL;
}

time_t rtc_read(void)
{
    return rtc_ticker_read() + rtc_offset;
}

void rtc_write(time_t t)
{
    rtc_offset = t - UNIX_OFFSET - rtc_ticker_read();
}
