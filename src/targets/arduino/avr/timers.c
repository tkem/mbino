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

#include "timers.h"

volatile uint8_t *const PROGMEM timer_to_tccr_PGM[] = {
#if defined(TCCR0)
    [TIMER0A] = &TCCR0,
#endif
#if defined(TCCR0A) && defined(COM0A1)
    [TIMER0A] = &TCCR0A,
#endif
#if defined(TCCR0A) && defined(COM0B1)
    [TIMER0B] = &TCCR0A,
#endif
#if defined(TCCR1A) && defined(COM1A1)
    [TIMER1A] = &TCCR1A,
#endif
#if defined(TCCR1A) && defined(COM1B1)
    [TIMER1B] = &TCCR1A,
#endif
#if defined(TCCR1A) && defined(COM1C1)
    [TIMER1C] = &TCCR1A,
#endif
#if defined(TCCR2)
    [TIMER2] = &TCCR2,
#endif
#if defined(TCCR2A) && defined(COM2A1)
    [TIMER2A] = &TCCR2A,
#endif
#if defined(TCCR2A) && defined(COM2B1)
    [TIMER2B] = &TCCR2A,
#endif
#if defined(TCCR3A) && defined(COM3A1)
    [TIMER3A] = &TCCR3A,
#endif
#if defined(TCCR3A) && defined(COM3B1)
    [TIMER3B] = &TCCR3A,
#endif
#if defined(TCCR3A) && defined(COM3C1)
    [TIMER3C] = &TCCR3A,
#endif
#if defined(TCCR4A) && defined(COM4A1)
    [TIMER4A] = &TCCR4A,
#endif
#if defined(TCCR4A) && defined(COM4B1)
    [TIMER4B] = &TCCR4A,
#endif
#if defined(TCCR4A) && defined(COM4C1)
    [TIMER4C] = &TCCR4A,
#endif
#if defined(TCCR4C) && defined(COM4D1)
    [TIMER4D] = &TCCR4C,
#endif
#if defined(TCCR5A) && defined(COM5A1)
    [TIMER5A] = &TCCR5A,
#endif
#if defined(TCCR5A) && defined(COM5B1)
    [TIMER5B] = &TCCR5A,
#endif
#if defined(TCCR5A) && defined(COM5C1)
    [TIMER5C] = &TCCR5A
#endif
};

volatile void *const PROGMEM timer_to_ocr_PGM[] = {
#if defined(OCR0)
    [TIMER0A] = &OCR0,
#endif
#if defined(OCR0A)
    [TIMER0A] = &OCR0A,
#endif
#if defined(OCR0B)
    [TIMER0B] = &OCR0B,
#endif
#if defined(OCR1A)
    [TIMER1A] = &OCR1A,
#endif
#if defined(OCR1B)
    [TIMER1B] = &OCR1B,
#endif
#if defined(OCR1C)
    [TIMER1C] = &OCR1C,
#endif
#if defined(OCR2)
    [TIMER2] = &OCR2,
#endif
#if defined(OCR2A)
    [TIMER2A] = &OCR2A,
#endif
#if defined(OCR2B)
    [TIMER2B] = &OCR2B,
#endif
#if defined(OCR3A)
    [TIMER3A] = &OCR3A,
#endif
#if defined(OCR3B)
    [TIMER3B] = &OCR3B,
#endif
#if defined(OCR3C)
    [TIMER3C] = &OCR3C,
#endif
#if defined(OCR4A)
    [TIMER4A] = &OCR4A,
#endif
#if defined(OCR4B)
    [TIMER4B] = &OCR4B,
#endif
#if defined(OCR4C)
    [TIMER4C] = &OCR4C,
#endif
#if defined(OCR4D)
    [TIMER4D] = &OCR4D,
#endif
#if defined(OCR5A)
    [TIMER5A] = &OCR5A,
#endif
#if defined(OCR5B)
    [TIMER5B] = &OCR5B,
#endif
#if defined(OCR5C)
    [TIMER5C] = &OCR5C
#endif
};

const uint8_t PROGMEM timer_to_com_mask_PGM[] = {
#if defined(COM00) && defined(COM01)
    [TIMER0A] = _BV(COM00) | _BV(COM01)
#endif
#if defined(COM0A0) && defined(COM0A1)
    [TIMER0A] = _BV(COM0A0) | _BV(COM0A1),
#endif
#if defined(COM0B0) && defined(COM0B1)
    [TIMER0B] = _BV(COM0B0) | _BV(COM0B1),
#endif
#if defined(COM1A0) && defined(COM1A1)
    [TIMER1A] = _BV(COM1A0) | _BV(COM1A1),
#endif
#if defined(COM1B0) && defined(COM1B1)
    [TIMER1B] = _BV(COM1B0) | _BV(COM1B1),
#endif
#if defined(COM1C0) && defined(COM1C1)
    [TIMER1C] = _BV(COM1C0) | _BV(COM1C1),
#endif
#if defined(COM20) && defined(COM21)
    [TIMER2] = _BV(COM20) | _BV(COM21),
#endif
#if defined(COM2A0) && defined(COM2A1)
    [TIMER2A] = _BV(COM2A0) | _BV(COM2A1),
#endif
#if defined(COM2B0) && defined(COM2B1)
    [TIMER2B] = _BV(COM2B0) | _BV(COM2B1),
#endif
#if defined(COM3A0) && defined(COM3A1)
    [TIMER3A] = _BV(COM3A0) | _BV(COM3A1),
#endif
#if defined(COM3B0) && defined(COM3B1)
    [TIMER3B] = _BV(COM3B0) | _BV(COM3B1),
#endif
#if defined(COM3C0) && defined(COM3C1)
    [TIMER3C] = _BV(COM3C0) | _BV(COM3C1),
#endif
#if defined(COM4A0) && defined(COM4A1)
    [TIMER4A] = _BV(COM4A0) | _BV(COM4A1),
#endif
#if defined(COM4B0) && defined(COM4B1)
    [TIMER4B] = _BV(COM4B0) | _BV(COM4B1),
#endif
#if defined(COM4C0) && defined(COM4C1)
    [TIMER4C] = _BV(COM4C0) | _BV(COM4C1),
#endif
#if defined(COM4D0) && defined(COM4D1)
    [TIMER4D] = _BV(COM4D0) | _BV(COM4D1),
#endif
#if defined(COM5A0) && defined(COM5A1)
    [TIMER5A] = _BV(COM5A0) | _BV(COM5A1),
#endif
#if defined(COM5B0) && defined(COM5B1)
    [TIMER5B] = _BV(COM5B0) | _BV(COM5B1),
#endif
#if defined(COM5C0) && defined(COM5C1)
    [TIMER5C] = _BV(COM5C0) | _BV(COM5C1)
#endif
};

#endif
