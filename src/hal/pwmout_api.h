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
#ifndef MBINO_PWMOUT_API_H
#define MBINO_PWMOUT_API_H

#include "platform/platform.h"

#if DEVICE_PWMOUT

// TBD: extern "C"?
namespace mbino {

    struct pwmout_interface_t;

    struct pwmout16_t {
        volatile uint8_t* tccra;
        volatile uint8_t* tccrb;
        volatile uint16_t* ocr;
        volatile uint16_t* icr;
        uint8_t com;
    };

    struct pwmout8_t {
        volatile uint8_t* ocr;
        volatile uint8_t* tccr;
        volatile uint8_t* output;
        uint8_t com;
        uint8_t mask;
    };

    union pwmout_object_t {
        pwmout16_t pwm16;
        pwmout8_t pwm8;
    };

    struct pwmout_t {
        const pwmout_interface_t* interface;
        pwmout_object_t object;
    };

    void pwmout_init(pwmout_t* obj, PinName pin);

    void pwmout_write_u16(pwmout_t *obj, uint16_t value);

    inline void pwmout_write(pwmout_t *obj, float value) {
        if (value <= 0.0f) {
            pwmout_write_u16(obj, 0);
        } else if (value >= 1.0f) {
            pwmout_write_u16(obj, 0xffff);
        } else {
            pwmout_write_u16(obj, value * 65535.0f);
        }
    }

    uint16_t pwmout_read_u16(pwmout_t* obj);

    inline float pwmout_read(pwmout_t* obj) {
        return pwmout_read_u16(obj) * (1.0f / 65535.0f);
    }

    void pwmout_period_us(pwmout_t* obj, long us);

    inline void pwmout_period_ms(pwmout_t* obj, long ms) {
        pwmout_period_us(obj, ms * 1000);
    }

    inline void pwmout_period(pwmout_t* obj, float seconds) {
        pwmout_period_us(obj, seconds * 1000000.0f);
    }

    void pwmout_pulsewidth_us(pwmout_t* obj, long us);

    inline void pwmout_pulsewidth_ms(pwmout_t* obj, long ms) {
        pwmout_pulsewidth_us(obj, ms * 1000);
    }

    inline void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
        pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
    }

}

#endif

#endif
