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
#ifndef MBINO_TIMERS_H
#define MBINO_TIMERS_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C"{
#endif

extern volatile uint8_t *const PROGMEM timer_to_tccr_PGM[];
extern volatile void *const PROGMEM timer_to_ocr_PGM[];
extern const uint8_t PROGMEM timer_to_com_mask_PGM[];

#define timerToControlRegister(T) ((volatile uint8_t *)(pgm_read_ptr(timer_to_tccr_PGM + (T))))
#define timerToOutputCompareRegister(T) ((volatile void *)(pgm_read_ptr(timer_to_ocr_PGM + (T))))
#define timerToCompareOutputModeMask(T) (pgm_read_byte(timer_to_com_mask_PGM + (T)))

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
