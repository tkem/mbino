/* mbino - mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
 *
 * mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "platform/mbed_critical.h"
#include "platform/mbed_toolchain.h"
#include "hal/critical_section_api.h"

// mbino limits this to 8 bit for atomic access on AVR, and since 255
// recursive invocations should be enough...
static volatile uint8_t critical_section_reentrancy_counter = 0;

// no matching Arduino API, therefore moved to target
//bool core_util_are_interrupts_enabled(void);
//bool core_util_is_isr_active(void);

bool core_util_in_critical_section(void)
{
    return hal_in_critical_section();
}

void core_util_critical_section_enter(void)
{
    hal_critical_section_enter();
    ++critical_section_reentrancy_counter;
}

void core_util_critical_section_exit(void)
{
    if (--critical_section_reentrancy_counter == 0) {
        hal_critical_section_exit();
    }
}

// TODO: MBED_WEAK doesn't work for symbols redefined in the same library - move all to HAL?

template<typename T>
static bool core_util_atomic_cas(volatile T* ptr, T* expectedCurrentValue, T desiredValue)
{
    bool success;
    T currentValue;
    core_util_critical_section_enter();
    currentValue = *ptr;
    if (currentValue == *expectedCurrentValue) {
        *ptr = desiredValue;
        success = true;
    } else {
        *expectedCurrentValue = currentValue;
        success = false;
    }
    core_util_critical_section_exit();
    return success;
}

template<typename T, typename U>
static T core_util_atomic_incr(volatile T* valuePtr, U delta)
{
    T newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr + delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

template<typename T, typename U>
static T core_util_atomic_decr(volatile T* valuePtr, U delta)
{
    T newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr - delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

MBED_WEAK bool core_util_atomic_cas_u8(volatile uint8_t* ptr, uint8_t* expectedCurrentValue, uint8_t desiredValue)
{
    return core_util_atomic_cas(ptr, expectedCurrentValue, desiredValue);
}

MBED_WEAK bool core_util_atomic_cas_u16(volatile uint16_t* ptr, uint16_t* expectedCurrentValue, uint16_t desiredValue)
{
    return core_util_atomic_cas(ptr, expectedCurrentValue, desiredValue);
}

MBED_WEAK bool core_util_atomic_cas_u32(volatile uint32_t* ptr, uint32_t* expectedCurrentValue, uint32_t desiredValue)
{
    return core_util_atomic_cas(ptr, expectedCurrentValue, desiredValue);
}

MBED_WEAK bool core_util_atomic_cas_ptr(void* volatile* ptr, void** expectedCurrentValue, void* desiredValue)
{
    return core_util_atomic_cas(ptr, expectedCurrentValue, desiredValue);
}

MBED_WEAK uint8_t core_util_atomic_incr_u8(volatile uint8_t* valuePtr, uint8_t delta)
{
    return core_util_atomic_incr(valuePtr, delta);
}

MBED_WEAK uint16_t core_util_atomic_incr_u16(volatile uint16_t* valuePtr, uint16_t delta)
{
    return core_util_atomic_incr(valuePtr, delta);
}

MBED_WEAK uint32_t core_util_atomic_incr_u32(volatile uint32_t* valuePtr, uint32_t delta)
{
    return core_util_atomic_incr(valuePtr, delta);
}

MBED_WEAK void* core_util_atomic_incr_ptr(void* volatile* valuePtr, ptrdiff_t delta)
{
    return core_util_atomic_incr(reinterpret_cast<char* volatile*>(valuePtr), delta);
}

MBED_WEAK uint8_t core_util_atomic_decr_u8(volatile uint8_t* valuePtr, uint8_t delta)
{
    return core_util_atomic_decr(valuePtr, delta);
}

MBED_WEAK uint16_t core_util_atomic_decr_u16(volatile uint16_t* valuePtr, uint16_t delta)
{
    return core_util_atomic_decr(valuePtr, delta);
}

MBED_WEAK uint32_t core_util_atomic_decr_u32(volatile uint32_t* valuePtr, uint32_t delta)
{
    return core_util_atomic_decr(valuePtr, delta);
}

MBED_WEAK void* core_util_atomic_decr_ptr(void* volatile* valuePtr, ptrdiff_t delta)
{
    return core_util_atomic_decr(reinterpret_cast<char* volatile*>(valuePtr), delta);
}
