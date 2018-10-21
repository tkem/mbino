/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2018 Thomas Kemmer
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

#include "avr_usarts.h"

volatile uint8_t *const PROGMEM usart_to_udr_PGM[] = {
#if defined(UDR)
    [0] = &UDR,
#endif
#if defined(UDR0)
    [0] = &UDR0,
#endif
#if defined(UDR1)
    [1] = &UDR1,
#endif
#if defined(UDR2)
    [2] = &UDR2,
#endif
#if defined(UDR3)
    [3] = &UDR3,
#endif
};

volatile uint8_t *const PROGMEM usart_to_ucsra_PGM[] = {
#if defined(UCSRA)
    [0] = &UCSRA,
#endif
#if defined(UCSR0A)
    [0] = &UCSR0A,
#endif
#if defined(UCSR1A)
    [1] = &UCSR1A,
#endif
#if defined(UCSR2A)
    [2] = &UCSR2A,
#endif
#if defined(UCSR3A)
    [3] = &UCSR3A,
#endif
};

volatile uint8_t *const PROGMEM usart_to_ucsrb_PGM[] = {
#if defined(UCSRB)
    [0] = &UCSRB,
#endif
#if defined(UCSR0B)
    [0] = &UCSR0B,
#endif
#if defined(UCSR1B)
    [1] = &UCSR1B,
#endif
#if defined(UCSR2B)
    [2] = &UCSR2B,
#endif
#if defined(UCSR3B)
    [3] = &UCSR3B,
#endif
};

volatile uint8_t *const PROGMEM usart_to_ucsrc_PGM[] = {
#if defined(UCSRC)
    [0] = &UCSRC,
#endif
#if defined(UCSR0C)
    [0] = &UCSR0C,
#endif
#if defined(UCSR1C)
    [1] = &UCSR1C,
#endif
#if defined(UCSR2C)
    [2] = &UCSR2C,
#endif
#if defined(UCSR3C)
    [3] = &UCSR3C,
#endif
};

volatile uint8_t *const PROGMEM usart_to_ubrrl_PGM[] = {
#if defined(UBRRL)
    [0] = &UBRRL,
#endif
#if defined(UBRR0L)
    [0] = &UBRR0L,
#endif
#if defined(UBRR1L)
    [1] = &UBRR1L,
#endif
#if defined(UBRR2L)
    [2] = &UBRR2L,
#endif
#if defined(UBRR3L)
    [3] = &UBRR3L,
#endif
};

volatile uint8_t *const PROGMEM usart_to_ubrrh_PGM[] = {
#if defined(UBRRH)
    [0] = &UBRRH,
#endif
#if defined(UBRR0H)
    [0] = &UBRR0H,
#endif
#if defined(UBRR1H)
    [1] = &UBRR1H,
#endif
#if defined(UBRR2H)
    [2] = &UBRR2H,
#endif
#if defined(UBRR3H)
    [3] = &UBRR3H,
#endif
};

#endif
