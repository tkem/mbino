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

template<typename T>
static bool core_util_atomic_cas(T* ptr, T* expectedCurrentValue, T desiredValue)
{
    bool result;
    uint8_t sreg = SREG;
    cli();
    if (*ptr == *expectedCurrentValue) {
        *ptr = desiredValue;
        result = true;
    } else {
        *expectedCurrentValue = *ptr;
        result = false;
    }
    SREG = sreg;
    return result;
}

template<typename T, typename U>
static T core_util_atomic_incr(T* valuePtr, U delta)
{
    T value;
    uint8_t sreg = SREG;
    cli();
    value = *valuePtr += delta;
    SREG = sreg;
    return value;
}

template<typename T, typename U>
static T core_util_atomic_decr(T* valuePtr, U delta)
{
    T value;
    uint8_t sreg = SREG;
    cli();
    value = *valuePtr -= delta;
    SREG = sreg;
    return value;
}

bool core_util_are_interrupts_enabled(void)
{
    return SREG & 0x80;
}

//bool core_util_is_isr_active(void);

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

bool core_util_atomic_cas_u8(uint8_t* ptr, uint8_t* expectedCurrentValue, uint8_t desiredValue)
{
    return core_util_atomic_cas(ptr, expectedCurrentValue, desiredValue);
}

bool core_util_atomic_cas_u16(uint16_t* ptr, uint16_t* expectedCurrentValue, uint16_t desiredValue)
{
    return core_util_atomic_cas(ptr, expectedCurrentValue, desiredValue);
}

bool core_util_atomic_cas_u32(uint32_t* ptr, uint32_t* expectedCurrentValue, uint32_t desiredValue)
{
    return core_util_atomic_cas(ptr, expectedCurrentValue, desiredValue);
}

bool core_util_atomic_cas_ptr(void** ptr, void** expectedCurrentValue, void* desiredValue)
{
    return core_util_atomic_cas(ptr, expectedCurrentValue, desiredValue);
}

uint8_t core_util_atomic_incr_u8(uint8_t* valuePtr, uint8_t delta)
{
    return core_util_atomic_incr(valuePtr, delta);
}

uint16_t core_util_atomic_incr_u16(uint16_t* valuePtr, uint16_t delta)
{
    return core_util_atomic_incr(valuePtr, delta);
}

uint32_t core_util_atomic_incr_u32(uint32_t* valuePtr, uint32_t delta)
{
    return core_util_atomic_incr(valuePtr, delta);
}

void* core_util_atomic_incr_ptr(void** valuePtr, ptrdiff_t delta)
{
    return core_util_atomic_incr(reinterpret_cast<char**>(valuePtr), delta);
}

uint8_t core_util_atomic_decr_u8(uint8_t* valuePtr, uint8_t delta)
{
    return core_util_atomic_decr(valuePtr, delta);
}

uint16_t core_util_atomic_decr_u16(uint16_t* valuePtr, uint16_t delta)
{
    return core_util_atomic_decr(valuePtr, delta);
}

uint32_t core_util_atomic_decr_u32(uint32_t* valuePtr, uint32_t delta)
{
    return core_util_atomic_decr(valuePtr, delta);
}

void* core_util_atomic_decr_ptr(void** valuePtr, ptrdiff_t delta)
{
    return core_util_atomic_decr(reinterpret_cast<char**>(valuePtr), delta);
}

#endif
