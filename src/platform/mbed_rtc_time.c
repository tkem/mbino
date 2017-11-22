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
#include "platform/mbed_rtc_time.h"
#include "platform/mbed_critical.h"
#include "hal/rtc_api.h"

#include <time.h>

static struct {
    void (*init)(void);
    int (*isenabled)(void);
    time_t (*read)(void);
    void (*write)(time_t t);
} rtc = {
#if DEVICE_RTC
    rtc_init, rtc_isenabled, rtc_read, rtc_write
#else
    NULL, NULL, NULL, NULL
#endif
};

time_t time(time_t *timer)
{
    time_t t = (time_t)-1;
    core_util_critical_section_enter();
    if (rtc.isenabled && !rtc.isenabled()) {
        set_time(0);
    }
    if (rtc.read) {
        t = rtc.read();
    }
    core_util_critical_section_exit();
    if (timer) {
        *timer = t;
    }
    return t;
}

void set_time(time_t t)
{
    core_util_critical_section_enter();
    if (rtc.init) {
        rtc.init();
    }
    if (rtc.write) {
        rtc.write(t);
    }
    core_util_critical_section_exit();
}

void attach_rtc(time_t (*read)(void), void (*write)(time_t), void (*init)(void), int (*isenabled)(void))
{
    core_util_critical_section_enter();
    rtc.read = read;
    rtc.write = write;
    rtc.init = init;
    rtc.isenabled = isenabled;
    core_util_critical_section_exit();
}
