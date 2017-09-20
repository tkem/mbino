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
#ifndef MBINO_UTIL_CRITICAL_H
#define MBINO_UTIL_CRITICAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

bool core_util_are_interrupts_enabled(void);

bool core_util_is_isr_active(void);

void core_util_critical_section_enter(void);

void core_util_critical_section_exit(void);

bool core_util_atomic_cas_u8(uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue);

bool core_util_atomic_cas_u16(uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue);

bool core_util_atomic_cas_u32(uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue);

bool core_util_atomic_cas_ptr(void **ptr, void **expectedCurrentValue, void *desiredValue);

uint8_t core_util_atomic_incr_u8(uint8_t *valuePtr, uint8_t delta);

uint16_t core_util_atomic_incr_u16(uint16_t *valuePtr, uint16_t delta);

uint32_t core_util_atomic_incr_u32(uint32_t *valuePtr, uint32_t delta);

void *core_util_atomic_incr_ptr(void **valuePtr, ptrdiff_t delta);

uint8_t core_util_atomic_decr_u8(uint8_t *valuePtr, uint8_t delta);

uint16_t core_util_atomic_decr_u16(uint16_t *valuePtr, uint16_t delta);

uint32_t core_util_atomic_decr_u32(uint32_t *valuePtr, uint32_t delta);

void *core_util_atomic_decr_ptr(void **valuePtr, ptrdiff_t delta);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
