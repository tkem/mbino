
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

#include <Arduino.h>

#include <limits.h>

#include "hal/pwmout_api.h"
#include "platform/mbino_error.h"
#include "avr_timers.h"

#define PWMOUT_INVARG MBED_MAKE_ERROR(MBED_MODULE_DRIVER_PWM, MBED_ERROR_CODE_INVALID_ARGUMENT)

// mbed pwmout default period is 20 ms (used for 16-bit timers only)
#define PWMOUT_DEFAULT_PERIOD 20000UL

// assume these are the same for all 16-bit timers
#define PWMOUT16_TCCRA_WGM_MASK 0x03    /* ((1 << WGM11) | (1 << WGM10)) */
#define PWMOUT16_TCCRB_WGM 0x10         /* WGM = 8 (phase and frequency correct PWM) */
#define PWMOUT16_TCCRB_CS_MASK 0x07     /* ((1 << CS12) | (1 << CS11) | (1 << CS10)) */

#if (microsecondsToClockCycles(PWMOUT_DEFAULT_PERIOD / 2) / 8) <= UINT16_MAX
#define PWMOUT16_ICR_INIT (microsecondsToClockCycles(PWMOUT_DEFAULT_PERIOD / 2) / 8)
#define PWMOUT16_TCCRB_INIT (PWMOUT16_TCCRB_WGM | 0x02)  /* CS = 2 (clk/8) */
#else
#error "Unsupported PWM default period."
#endif

#define PWMOUT_MAX_US (INT_MAX * 0.000001f)

// prescale shift values for CSn2:0 (0: timer stopped; 6, 7: external clock)
static const uint8_t pwmout16_cs_to_prescale[8] = {0xff, 0, 3, 6, 8, 10, 0, 0};

static uint16_t pwmout16_read(pwmout_object_t *obj)
{
    pwmout16_t *pwm = &obj->pwm16;
    uint16_t value = *pwm->ocr;
    uint16_t top = *pwm->icr;
    if (value >= top) {
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
    uint32_t r = value;
    r *= top;
    r /= UINT16_MAX;
    *pwm->ocr = r;
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
    // save and restore duty cycle
    uint16_t duty = pwmout16_read(obj);
    *pwm->icr = top;
    *pwm->tccrb = PWMOUT16_TCCRB_WGM | cs;
    pwmout16_write(obj, duty);
}

static void pwmout16_pulsewidth(pwmout_object_t *obj, uint32_t value)
{
    pwmout16_t *pwm = &obj->pwm16;
    value >>= pwmout16_cs_to_prescale[*pwm->tccrb & PWMOUT16_TCCRB_CS_MASK];
    *pwm->ocr = min(value, UINT16_MAX);
}

static void pwmout16_free(pwmout_object_t *obj)
{
    pwmout16_t *pwm = &obj->pwm16;
    *pwm->tccra &= ~pwm->com;
}

static void pwmout16_init(pwmout_t *obj,
                          volatile uint16_t *ocr,
                          volatile uint16_t *icr,
                          volatile uint8_t *tccra,
                          volatile uint8_t *tccrb,
                          uint8_t com)
{
    static const pwmout_interface_t interface = {
        pwmout16_read,
        pwmout16_write,
        pwmout16_period,
        pwmout16_pulsewidth,
        pwmout16_free
    };
    obj->interface = &interface;

    pwmout16_t *pwm = &obj->object.pwm16;
    pwm->ocr = ocr;
    pwm->icr = icr;
    pwm->tccra = tccra;
    pwm->tccrb = tccrb;
    pwm->com = com;

    // clear rightmost 1-bit in COM mask
    uint8_t com10 = (com & (com - 1));

    uint8_t sreg = SREG;
    cli();
    *ocr = 0;
    *icr = PWMOUT16_ICR_INIT;
    *tccrb = PWMOUT16_TCCRB_INIT;
    *tccra = (*tccra & ~(PWMOUT16_TCCRA_WGM_MASK | com)) | com10;
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
        // clear rightmost 1-bit in COM mask
        *pwm->tccr |= pwm->com & (pwm->com - 1);
    }
}

static void pwmout8_notavail(pwmout_object_t *obj, uint32_t value)
{
    // pulsewidth and period are ignored on 8-bit timers
}

static void pwmout8_free(pwmout_object_t *obj)
{
    pwmout8_t *pwm = &obj->pwm8;
    *pwm->tccr &= ~pwm->com;
}

static void pwmout8_init(pwmout_t *obj,
                         volatile uint8_t *ocr,
                         volatile uint8_t *tccr,
                         volatile uint8_t *output,
                         uint8_t com,
                         uint8_t mask)
{
    static const pwmout_interface_t pwmout8_interface = {
        pwmout8_read,
        pwmout8_write,
        pwmout8_notavail,
        pwmout8_notavail,
        pwmout8_free
    };
    obj->interface = &pwmout8_interface;

    pwmout8_t *pwm = &obj->object.pwm8;
    pwm->ocr = ocr;
    pwm->tccr = tccr;
    pwm->output = output;
    pwm->com = com;
    pwm->mask = mask;
}

void pwmout_init(pwmout_t *obj, PinName pin)
{
    uint8_t timer = digitalPinToTimer(pin);
    if (timer == NOT_ON_TIMER) {
        MBINO_ERROR1(PWMOUT_INVARG, "Not a PWM-enabled pin", pin);
    }
    // set as output
    uint8_t port = digitalPinToPort(pin);
    uint8_t mask = digitalPinToBitMask(pin);
    volatile uint8_t *mode = portModeRegister(port);
    volatile uint8_t *output = portOutputRegister(port);
    uint8_t sreg = SREG;
    cli();
    *output &= ~mask;
    *mode |= mask;
    SREG = sreg;

    volatile uint8_t *tccr = timerToControlRegister(timer);
    volatile void *ocr = timerToOutputCompareRegister(timer);
    uint8_t com = timerToCompareOutputModeMask(timer);

    if (false) {
        // keep the compiler happy
#if defined(TCCR1A) && defined(TCCR1B) && defined(ICR1)
    } else if (tccr == &TCCR1A) {
        pwmout16_init(obj, (volatile uint16_t *)ocr, &ICR1, &TCCR1A, &TCCR1B, com);
#endif
#if defined(TCCR3A) && defined(TCCR3B) && defined(ICR3)
    } else if (tccr == &TCCR3A) {
        pwmout16_init(obj, (volatile uint16_t *)ocr, &ICR3, &TCCR3A, &TCCR3B, com);
#endif
#if defined(TCCR4A) && defined(TCCR4B) && defined(ICR4)
    } else if (tccr == &TCCR4A) {
        pwmout16_init(obj, (volatile uint16_t *)ocr, &ICR4, &TCCR4A, &TCCR4B, com);
#endif
#if defined(TCCR5A) && defined(TCCR5B) && defined(ICR5)
    } else if (tccr == &TCCR5A) {
        pwmout16_init(obj, (volatile uint16_t *)ocr, &ICR5, &TCCR5A, &TCCR5B, com);
#endif
    } else {
        pwmout8_init(obj, (volatile uint8_t *)ocr, tccr, output, com, mask);
    }
}

void pwmout_free(pwmout_t *obj)
{
    uint8_t sreg = SREG;
    cli();
    obj->interface->free(&obj->object);
    SREG = sreg;
}

void pwmout_write(pwmout_t *obj, float percent)
{
    uint16_t value;
    if (percent < 0.0f) {
        value = 0;
    } else if (percent > 1.0f) {
        value = UINT16_MAX;
    } else {
        value = percent * (float)UINT16_MAX;
    }
    uint8_t sreg = SREG;
    cli();
    obj->interface->write(&obj->object, value);
    SREG = sreg;
}

float pwmout_read(pwmout_t *obj)
{
    uint16_t value;
    uint8_t sreg = SREG;
    cli();
    value = obj->interface->read(&obj->object);
    SREG = sreg;
    return value * (1.0f / (float)UINT16_MAX);
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    if (seconds > PWMOUT_MAX_US) {
        pwmout_period_ms(obj, seconds * 1000.0f);
    } else {
        pwmout_period_us(obj, seconds * 1000000.0f);
    }
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    uint32_t cycles = microsecondsToClockCycles(ms * 500UL);
    uint8_t sreg = SREG;
    cli();
    obj->interface->period(&obj->object, cycles);
    SREG = sreg;
}

void pwmout_period_us(pwmout_t *obj, int us)
{
    uint32_t cycles = microsecondsToClockCycles(us / 2);
    uint8_t sreg = SREG;
    cli();
    obj->interface->period(&obj->object, cycles);
    SREG = sreg;
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    if (seconds > PWMOUT_MAX_US) {
        pwmout_pulsewidth_ms(obj, seconds * 1000.0f);
    } else {
        pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
    }
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    uint32_t cycles = microsecondsToClockCycles(ms * 500UL);
    uint8_t sreg = SREG;
    cli();
    obj->interface->pulsewidth(&obj->object, cycles);
    SREG = sreg;
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    uint32_t cycles = microsecondsToClockCycles(us / 2);
    uint8_t sreg = SREG;
    cli();
    obj->interface->pulsewidth(&obj->object, cycles);
    SREG = sreg;
}

#endif
