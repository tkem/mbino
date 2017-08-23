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
#include "pwmout_api.h"

#if DEVICE_PWMOUT

#include <Arduino.h>
#include <stdint.h>

namespace mbino {

    struct pwmtimer_t {
        uint8_t id;  // TBD
        volatile uint8_t* tccra;
        volatile uint8_t* tccrb;
        volatile uint16_t* icr;
    };

    // TODO: PROGMEM?
#if defined(TCCR0A) && defined(TCCR0B)
    static const pwmtimer_t timer_TCCR0A = { 0, &TCCR0A, &TCCR0B, 0 };
#endif

#if defined(TCCR1A) && defined(TCCR1B)
    static const pwmtimer_t timer_TCCR1A = { 1, &TCCR1A, &TCCR1B, &ICR1 };
#endif

#if defined(TCCR1A) && !defined(TCCR1B)
    static const pwmtimer_t timer_TCCR1A = { 1 };
#endif
#if defined(TCCR2A) && defined(TCCR2B)
    static const pwmtimer_t timer_TCCR2A = { 2, &TCCR2A, &TCCR2B, 0 };
#endif

#if defined(TCCR3A) && defined(TCCR3B)
    static const pwmtimer_t timer_TCCR3A = { 3, &TCCR3A, &TCCR3B, &ICR3 };
#endif

#if defined(TCCR4A) && defined(TCCR4B)
    static const pwmtimer_t timer_TCCR4A = {
        4,
        &TCCR4A,
        &TCCR4B,
#ifdef ICR4
        &ICR4
#endif
    };
#endif

#if defined(TCCR4C) && defined(TCCR4D)
    static const pwmtimer_t timer_TCCR4C = { 4, &TCCR4C, &TCCR4D, 0 };
#endif

#if defined(TCCR5A) && defined(TCCR5B)
    static const pwmtimer_t timer_TCCR5A = { 5, &TCCR5A, &TCCR5B, &ICR5 };
#endif

    static bool pwmtimer_enable_pwm(const pwmtimer_t* timer, uint8_t com)
    {
#if defined(WGM10) && defined(WGM11) && defined(WGM12) && defined(WGM13)
        if (timer->icr) {
            *timer->icr = 0;
            // TODO: check that all 16-bit timers have same WGM values!
            *timer->tccra = (*timer->tccra & ~(_BV(WGM11) | _BV(WGM10))) | _BV(com);
            *timer->tccrb = (*timer->tccrb & ~_BV(WGM12)) | _BV(WGM13);
            return true;
        } else {
            return false;
        }
#else
        return false;
#endif
    }

    static bool pwmout_timer_init(pwmout_t* obj, const pwmtimer_t* timer,
                                  volatile uint8_t* ocr, uint8_t com1)
    {
        obj->timer = timer;
        obj->ocr = 0;
        obj->com = 0;
        if (timer->id == 0) {
            // assume prescale factor 64, 8-bit fast pwm mode; note
            // that "obj->period" is actually half the period for
            // phase correct pwm
            obj->period = 128;
            obj->prescale = 6;
        } else {
            // assume prescale factor 64, 8-bit phase correct pwm mode
            obj->period = 255;
            obj->prescale = 6;
        }
        obj->mode = 0;
        return true;
    }

    static bool pwmout_timer_init(pwmout_t* obj, const pwmtimer_t* timer,
                                  volatile uint16_t* ocr, uint8_t com1)
    {
        obj->timer = timer;
        obj->ocr = ocr;
        obj->com = com1;
        // assume prescale factor 64, 8-bit phase correct pwm mode
        obj->period = 255;
        obj->prescale = 6;
        obj->mode = 0;
        return true;
    }

    bool pwmout_init(pwmout_t* obj, PinName pin)
    {
        // do *not* call analogWrite() from contructor, since timers
        // may not be set up; at least, this sets it the pin as output
        obj->pin = pin;
        obj->value = 0;
        digitalWrite(pin, 0);
        pinMode(pin, OUTPUT);

        // Adapted from wiring_analog.c, Copyright (c) 2005-2006 David A. Mellis
        switch (digitalPinToTimer(pin)) {
#if defined(TCCR0A) && defined(COM0A1)
        case TIMER0A:
            return pwmout_timer_init(obj, &timer_TCCR0A, &OCR0A, COM0A1);
#endif

#if defined(TCCR0A) && defined(COM0B1)
        case TIMER0B:
            return pwmout_timer_init(obj, &timer_TCCR0A, &OCR0B, COM0B1);
#endif

#if defined(TCCR1A) && defined(COM1A1)
        case TIMER1A:
            return pwmout_timer_init(obj, &timer_TCCR1A, &OCR1A, COM1A1);
#endif

#if defined(TCCR1A) && defined(COM1B1)
        case TIMER1B:
            return pwmout_timer_init(obj, &timer_TCCR1A, &OCR1B, COM1B1);
#endif

#if defined(TCCR1A) && defined(COM1C1)
        case TIMER1C:
            return pwmout_timer_init(obj, &timer_TCCR1A, &OCR1C, COM1C1);
#endif

#if defined(TCCR2A) && defined(COM2A1)
        case TIMER2A:
            return pwmout_timer_init(obj, &timer_TCCR2A, &OCR2A, COM2A1);
#endif

#if defined(TCCR2A) && defined(COM2B1)
        case TIMER2B:
            return pwmout_timer_init(obj, &timer_TCCR2A, &OCR2B, COM2B1);
#endif

#if defined(TCCR3A) && defined(COM3A1)
        case TIMER3A:
            return pwmout_timer_init(obj, &timer_TCCR3A, &OCR3A, COM3A1);
#endif

#if defined(TCCR3A) && defined(COM3B1)
        case TIMER3B:
            return pwmout_timer_init(obj, &timer_TCCR3A, &OCR3B, COM3B1);
#endif

#if defined(TCCR3A) && defined(COM3C1)
        case TIMER3C:
            return pwmout_timer_init(obj, &timer_TCCR3A, &OCR3C, COM3C1);
#endif

#if defined(TCCR4A) && defined(COM4A1)
        case TIMER4A:
            return pwmout_timer_init(obj, &timer_TCCR4A, &OCR4A, COM4A1);
#endif

#if defined(TCCR4A) && defined(COM4B1)
        case TIMER4B:
            return pwmout_timer_init(obj, &timer_TCCR4A, &OCR4B, COM4B1);
#endif

#if defined(TCCR4A) && defined(COM4C1)
        case TIMER4C:
            return pwmout_timer_init(obj, &timer_TCCR4A, &OCR4C, COM4C1);
#endif

#if defined(TCCR4C) && defined(COM4D1)
        case TIMER4D:
            return pwmout_timer_init(obj, &timer_TCCR4C, &OCR4D, COM4D1);
#endif

#if defined(TCCR5A) && defined(COM5A1)
        case TIMER5A:
            return pwmout_timer_init(obj, &timer_TCCR5A, &OCR5A, COM5A1);
#endif

#if defined(TCCR5A) && defined(COM5B1)
        case TIMER5B:
            return pwmout_timer_init(obj, &timer_TCCR5A, &OCR5B, COM5B1);
#endif

#if defined(TCCR5A) && defined(COM5C1)
        case TIMER5C:
            return pwmout_timer_init(obj, &timer_TCCR5A, &OCR5C, COM5C1);
#endif
        default:
            return false;
        }
    }

    void pwmout_free(pwmout_t* obj)
    {
        digitalWrite(obj->pin, obj->value >= 128);
    }

    void pwmout_write_u16(pwmout_t* obj, uint16_t value)
    {
        uint8_t sreg = SREG;
        cli();
        obj->value = value >> 8;
        if (obj->mode == 0) {
            analogWrite(obj->pin, obj->value);
        } else {
            unsigned long duty = obj->period;
            duty *= value;
            *obj->ocr = duty >> 16;
        }
        SREG = sreg;
    }

    float pwmout_read(pwmout_t* obj)
    {
        // FIXME: use real value?
        return obj->value * (1.0f / 255.0f);
    }

    void pwmout_period_us(pwmout_t* obj, long us)
    {
        if (pwmtimer_enable_pwm(obj->timer, obj->com)) {
            // Adapted from TimerOne library: https://playground.arduino.cc/Code/Timer1
            // FIXME: check CSnX is same for all timers!
            unsigned long cycles = microsecondsToClockCycles(us / 2);
            uint8_t cs;
            uint8_t ps;
            if (cycles <= UINT16_MAX) {
                cs = _BV(CS10);
                ps = 0;
            } else if ((cycles >>= 3) <= UINT16_MAX) {
                cs = _BV(CS11);
                ps = 3;
            } else if ((cycles >>= 3) <= UINT16_MAX) {
                cs = _BV(CS11) | _BV(CS10);
                ps = 6;
            } else if ((cycles >>= 2) <= UINT16_MAX) {
                cs = _BV(CS12);
                ps = 8;
            } else if ((cycles >>= 2) <= UINT16_MAX) {
                cs = _BV(CS12) | _BV(CS10);
                ps = 10;
            } else {
                cs = _BV(CS12) | _BV(CS10);
                cycles = UINT16_MAX;
                ps = 10;
            }

            uint16_t value = 0;
            uint8_t sreg = SREG;
            cli();
            value = obj->value;
            obj->prescale = ps;
            *obj->timer->icr = obj->period = cycles;
            *obj->timer->tccrb &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
            *obj->timer->tccrb |= cs;
            obj->mode = 1;
            SREG = sreg;

            if (value != 0) {
                pwmout_write_u16(obj, (value << 8) | value);
            }
        }
    }

    void pwmout_pulsewidth_us(pwmout_t* obj, long us)
    {
        unsigned long value = microsecondsToClockCycles(us / 2);
        uint16_t period;
        uint8_t prescale;
        uint8_t sreg = SREG;
        cli();
        period = obj->period;
        prescale = obj->prescale;
        SREG = sreg;

        value >>= prescale;
        if (value >= period) {
            value = period - 1;
        }
        value <<= 16;
        value /= period;

        pwmout_write_u16(obj, value);
    }

}

#endif
