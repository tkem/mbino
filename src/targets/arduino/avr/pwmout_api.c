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

#include "hal/pwmout_api.h"

#include <Arduino.h>

#include "timers.h"

// prescale shift values for CSn2:0 (0: timer stopped; 6, 7: external clock)
static const uint8_t pwmout16_cs_to_prescale[8] = {0xff, 0, 3, 6, 8, 10, 0, 0};
// we just assume these are the same for all 16-bit timers
static const uint8_t pwmout16_tccra_wgm_mask = 0x03;
static const uint8_t pwmout16_tccrb_wgm_mask = 0x18;
// mode 8: Phase and frequency correct PWM with TOP = ICRn
static const uint8_t pwmout16_tccrb_wgm_flag = 0x10;
static const uint8_t pwmout16_tccrb_cs_mask = 0x07;

static uint16_t pwmout16_read(pwmout_object_t *obj)
{
    pwmout16_t *pwm = &obj->pwm16;
    uint16_t value = *pwm->ocr;
    uint16_t top = *pwm->icr;
    if (top == 0) {
        // TODO: always assume 8-bit phase correct PWM?
        return (value << 8) | value;
    } else if (value > top) {
        return UINT16_MAX;
    } else {
        uint32_t r = value;
        r *= UINT16_MAX;
        r /= top;
        return r;
    }
}

static void pwmout16_write(pwmout_object_t *obj, uint16_t value)
{
    pwmout16_t *pwm = &obj->pwm16;
    uint16_t top = *pwm->icr;
    if (top == 0) {
        // TODO: always assume 8-bit phase correct PWM?
        *pwm->ocr = value >> 8;
    } else {
        uint32_t r = value;
        r *= top;
        r /= UINT16_MAX;
        *pwm->ocr = r;
    }
}

static void pwmout16_period(pwmout_object_t *obj, uint32_t value)
{
    pwmout16_t *pwm = &obj->pwm16;
    uint16_t top = UINT16_MAX;
    uint8_t cs = 0;
    do {
        uint32_t v = value >> pwmout16_cs_to_prescale[++cs];
        if (v <= UINT16_MAX) {
            top = v;
            break;  // from loop
        }
    } while (cs != 5);
    // keep duty cycle
    uint16_t duty = pwmout16_read(obj);
    *pwm->icr = top;
    *pwm->tccra &= ~pwmout16_tccra_wgm_mask;
    *pwm->tccrb &= ~(pwmout16_tccrb_wgm_mask | pwmout16_tccrb_cs_mask);
    *pwm->tccrb |= (pwmout16_tccrb_wgm_flag | cs);
    pwmout16_write(obj, duty);
}

static void pwmout16_pulsewidth(pwmout_object_t *obj, uint32_t value)
{
    pwmout16_t *pwm = &obj->pwm16;
    value >>= pwmout16_cs_to_prescale[*pwm->tccrb & pwmout16_tccrb_cs_mask];
    *pwm->ocr = min(value, UINT16_MAX);
}

static const pwmout_interface_t pwmout16_interface = {
    pwmout16_read,
    pwmout16_write,
    pwmout16_period,
    pwmout16_pulsewidth
};

static void pwmout16_init(pwmout_t *obj,
                          volatile uint16_t *ocr,
                          uint8_t com,
                          volatile uint8_t *tccra,
                          volatile uint8_t *tccrb,
                          volatile uint16_t *icr)
{

    pwmout16_t *pwm = &obj->object.pwm16;
    pwm->ocr = ocr;
    pwm->com = com;
    pwm->tccra = tccra;
    pwm->tccrb = tccrb;
    pwm->icr = icr;
    obj->interface = &pwmout16_interface;

    uint8_t sreg = SREG;
    cli();
    // TODO: init tccrb? startup phase output?
    *ocr = 0;
    // turn off rightmost 1-bit in COM mask
    *tccra |= com & (com - 1);
    SREG = sreg;
}

static uint16_t pwmout8_read(pwmout_object_t *obj)
{
    pwmout8_t *pwm = &obj->pwm8;
    uint16_t value;
    if (*pwm->tccr & pwm->com) {
        value = *pwm->ocr;
    } else {
        value = (*pwm->output & pwm->mask) ? UINT8_MAX : 0;
    }
    // 8-bit to 16-bit conversion
    return (value << 8) | value;
}

static void pwmout8_write(pwmout_object_t *obj, uint16_t value)
{
    pwmout8_t *pwm = &obj->pwm8;
    if (value == 0) {
        *pwm->output &= ~pwm->mask;
        *pwm->tccr &= ~pwm->com;
    } else if (value == UINT16_MAX) {
        *pwm->output |= pwm->mask;
        *pwm->tccr &= ~pwm->com;
    } else {
        *pwm->ocr = value >> 8;
        // turn off rightmost 1-bit in COM mask
        *pwm->tccr |= pwm->com & (pwm->com - 1);
    }
}

static void pwmout8_nop(pwmout_object_t *obj, uint32_t value) {}

static const pwmout_interface_t pwmout8_interface = {
    pwmout8_read,
    pwmout8_write,
    pwmout8_nop,
    pwmout8_nop
};

static void pwmout8_init(pwmout_t *obj,
                         volatile uint8_t *ocr,
                         uint8_t com,
                         volatile uint8_t *tccr,
                         volatile uint8_t *output,
                         uint8_t mask)
{
    pwmout8_t *pwm = &obj->object.pwm8;
    pwm->ocr = ocr;
    pwm->com = com;
    pwm->tccr = tccr;
    pwm->output = output;
    pwm->mask = mask;
    obj->interface = &pwmout8_interface;
}

void pwmout_init(pwmout_t *obj, PinName pin)
{
    uint8_t port = digitalPinToPort(pin);
    uint8_t mask = digitalPinToBitMask(pin);
    volatile uint8_t *mode = portModeRegister(port);
    volatile uint8_t *output = portOutputRegister(port);

    uint8_t sreg = SREG;
    cli();
    *output &= ~mask;
    *mode |= mask;
    SREG = sreg;

    uint8_t timer = digitalPinToTimer(pin);
    // TODO: NOT_ON_TIMER?
    volatile uint8_t *tccr = timerToControlRegister(timer);
    volatile void *ocr = timerToOutputCompareRegister(timer);
    uint8_t com = timerToCompareOutputModeMask(timer);

    // TODO: think of something better...
    switch ((uint16_t)tccr) {
#if defined(TCCR1A) && defined(TCCR1B) && defined(ICR1)
    case (uint16_t)&TCCR1A:
        pwmout16_init(obj, (volatile uint16_t *)ocr, com, &TCCR1A, &TCCR1B, &ICR1);
        break;
#endif
#if defined(TCCR3A) && defined(TCCR3B) && defined(ICR3)
    case (uint16_t)&TCCR3A:
        pwmout16_init(obj, (volatile uint16_t *)ocr, com, &TCCR3A, &TCCR3B, &ICR3);
        break;
#endif
#if defined(TCCR4A) && defined(TCCR4B) && defined(ICR4)
    case (uint16_t)&TCCR4A:
        pwmout16_init(obj, (volatile uint16_t *)ocr, com, &TCCR4A, &TCCR4B, &ICR4);
        break;
#endif
#if defined(TCCR5A) && defined(TCCR5B) && defined(ICR5)
    case (uint16_t)&TCCR5A:
        pwmout16_init(obj, (volatile uint16_t *)ocr, com, &TCCR5A, &TCCR5B, &ICR5);
        break;
#endif
    default:
        pwmout8_init(obj, (volatile uint8_t *)ocr, com, tccr, output, mask);
    }
}

uint16_t pwmout_read_u16(pwmout_t *obj)
{
    uint16_t value;
    uint8_t sreg = SREG;
    cli();
    value = obj->interface->read(&obj->object);
    SREG = sreg;
    return value;
}

void pwmout_write_u16(pwmout_t *obj, uint16_t value)
{
    uint8_t sreg = SREG;
    cli();
    obj->interface->write(&obj->object, value);
    SREG = sreg;
}

void pwmout_period_us(pwmout_t *obj, long us)
{
    uint32_t cycles = microsecondsToClockCycles(us / 2);
    uint8_t sreg = SREG;
    cli();
    obj->interface->period(&obj->object, cycles);
    SREG = sreg;
}

void pwmout_pulsewidth_us(pwmout_t *obj, long us)
{
    uint32_t cycles = microsecondsToClockCycles(us / 2);
    uint8_t sreg = SREG;
    cli();
    obj->interface->pulsewidth(&obj->object, cycles);
    SREG = sreg;
}

#endif
