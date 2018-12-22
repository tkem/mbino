/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
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

#include "hal/serial_api.h"

#if DEVICE_SERIAL

#include "avr_usarts.h"

static uart_irq_handler serial_event_handler = 0;

static uint32_t serial_irq_ids[] = {
#if defined(USART0_RX_vect) || defined(USART0_UDRE_vect)
    [0] = 0,
#endif
#if defined(USART1_RX_vect) || defined(USART1_UDRE_vect)
    [1] = 0,
#endif
#if defined(USART2_RX_vect) || defined(USART2_UDRE_vect)
    [2] = 0,
#endif
#if defined(USART3_RX_vect) || defined(USART3_UDRE_vect)
    [3] = 0,
#endif
};

static void serial_event(int usart, SerialIrq irq)
{
    if (serial_event_handler) {
        serial_event_handler(serial_irq_ids[usart], irq);
    }
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    uint8_t sreg = SREG;
    cli();
    // this *really* sets the global handler...
    serial_event_handler = handler;
    serial_irq_ids[obj->usart] = id;
    SREG = sreg;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    uint8_t mask = 1 << (irq == RxIrq ? RXCIE : UDRIE);
    uint8_t sreg = SREG;
    cli();
    if (enable) {
        *obj->ucsrb |= mask;
    } else {
        *obj->ucsrb &= ~mask;
    }
    // TODO: disable pending IRQs, e.g. if obj gets destroyed
    SREG = sreg;
}

#ifdef USART0_RX_vect
ISR(USART0_RX_vect)
{
    serial_event(0, RxIrq);
}
#endif

#ifdef USART0_UDRE_vect
ISR(USART0_UDRE_vect)
{
    serial_event(0, TxIrq);
}
#endif

#ifdef USART1_RX_vect
ISR(USART1_RX_vect)
{
    serial_event(1, RxIrq);
}
#endif

#ifdef USART1_UDRE_vect
ISR(USART1_UDRE_vect)
{
    serial_event(1, TxIrq);
}
#endif

#ifdef USART2_RX_vect
ISR(USART2_RX_vect)
{
    serial_event(2, RxIrq);
}
#endif

#ifdef USART2_UDRE_vect
ISR(USART2_UDRE_vect)
{
    serial_event(2, TxIrq);
}
#endif

#ifdef USART3_RX_vect
ISR(USART3_RX_vect)
{
    serial_event(3, RxIrq);
}
#endif

#ifdef USART3_UDRE_vect
ISR(USART3_UDRE_vect)
{
    serial_event(3, TxIrq);
}
#endif

#endif

#endif
