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
#ifndef MBINO_AVR_USARTS_H
#define MBINO_AVR_USARTS_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(TXC)
#if defined(TXC0)
#define RXC RXC0
#define TXC TXC0
#define RXEN RXEN0
#define TXEN TXEN0
#define RXCIE RXCIE0
#define UDRIE UDRIE0
#define U2X U2X0
#define UPE UPE0
#define UDRE UDRE0
#define FE FE0
#elif defined(TXC1)
#define RXC RXC1
#define TXC TXC1
#define RXEN RXEN1
#define TXEN TXEN1
#define RXCIE RXCIE1
#define UDRIE UDRIE1
#define U2X U2X1
#define UPE UPE1
#define UDRE UDRE1
#define FE FE1
#endif
#endif

#if !defined(UPE)
#if defined(PE)
#define UPE PE
#endif
#endif

#if !defined(USART0_RX_vect)
#if defined(USART_RX_vect)
#define USART0_RX_vect USART_RX_vect
#elif defined(USART_RXC_vect)
#define USART0_RX_vect USART_RXC_vect
#endif
#endif

#if !defined(USART0_UDRE_vect)
#if defined(USART_UDRE_vect)
#define USART0_UDRE_vect USART_UDRE_vect
#elif defined(UART_UDRE_vect)
#define USART0_UDRE_vect UART_UDRE_vect
#elif defined(UART0_UDRE_vect)
#define USART0_UDRE_vect UART0_UDRE_vect
#endif
#endif

#if !defined(USART1_RX_vect)
#if defined(UART1_RX_vect)
#define USART1_RX_vect UART1_RX_vect
#endif
#endif

#if !defined(USART1_UDRE_vect)
#if defined(UART1_UDRE_vect)
#define USART1_UDRE_vect UART1_UDRE_vect
#endif
#endif

extern volatile uint8_t *const PROGMEM usart_to_udr_PGM[];
extern volatile uint8_t *const PROGMEM usart_to_ucsra_PGM[];
extern volatile uint8_t *const PROGMEM usart_to_ucsrb_PGM[];
extern volatile uint8_t *const PROGMEM usart_to_ucsrc_PGM[];
extern volatile uint8_t *const PROGMEM usart_to_ubrrl_PGM[];
extern volatile uint8_t *const PROGMEM usart_to_ubrrh_PGM[];

#define usartToDataRegister(N) ((volatile uint8_t *)(pgm_read_ptr(usart_to_udr_PGM + (N))))
#define usartToControlStatusRegisterA(N) ((volatile uint8_t *)(pgm_read_ptr(usart_to_ucsra_PGM + (N))))
#define usartToControlStatusRegisterB(N) ((volatile uint8_t *)(pgm_read_ptr(usart_to_ucsrb_PGM + (N))))
#define usartToControlStatusRegisterC(N) ((volatile uint8_t *)(pgm_read_ptr(usart_to_ucsrc_PGM + (N))))
#define usartToBaudRateRegisterL(N) ((volatile uint8_t *)(pgm_read_ptr(usart_to_ubrrl_PGM + (N))))
#define usartToBaudRateRegisterH(N) ((volatile uint8_t *)(pgm_read_ptr(usart_to_ubrrh_PGM + (N))))

#ifdef __cplusplus
}
#endif

#endif
