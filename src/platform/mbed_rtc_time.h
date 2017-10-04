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
#ifndef MBINO_MBED_RTC_TIME_H
#define MBINO_MBED_RTC_TIME_H

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

void set_time(time_t t);

void attach_rtc(time_t (*read)(void), void (*write)(time_t), void (*init)(void), int (*isenabled)(void));

#ifdef __cplusplus
}
#endif

#endif
