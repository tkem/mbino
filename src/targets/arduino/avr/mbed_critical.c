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

#include "platform/mbed_critical.h"

#include <Arduino.h>

static volatile uint8_t core_util_critical_section_counter = 0;
static volatile uint8_t core_util_critical_section_sreg;

void core_util_critical_section_enter(void)
{
    uint8_t sreg = SREG;
    cli();
    if (core_util_critical_section_counter++ == 0) {
        core_util_critical_section_sreg = sreg;
    }
}

void core_util_critical_section_exit(void)
{
    if (--core_util_critical_section_counter == 0) {
        SREG = core_util_critical_section_sreg;
    }
}

#endif
