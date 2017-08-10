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

// TBD: extern "C"?
namespace mbino {

    struct pwmout_t {
        uint8_t pin;
        uint8_t value;
    };

    void pwmout_init(pwmout_t* obj, PinName pin);

    void pwmout_free(pwmout_t *obj);

    void pwmout_write_u8(pwmout_t* obj, uint8_t value);

    inline void pwmout_write(pwmout_t *obj, float value) {
        if (value <= 0.0f) {
            pwmout_write_u8(obj, 0);
        } else if (value >= 1.0f) {
            pwmout_write_u8(obj, 255);
        } else {
            pwmout_write_u8(obj, value * 255.0f);
        }
    }

    inline float pwmout_read(pwmout_t* obj) {
        return obj->value / 255.0f;
    }

    void pwmout_period_us(pwmout_t* obj, long us);

    inline void pwmout_period_ms(pwmout_t* obj, long ms) {
        pwmout_period_us(obj, ms * 1000);
    }

    inline void pwmout_period(pwmout_t* obj, float seconds) {
        pwmout_period_us(obj, seconds / 1000000.0f);
    }

    void pwmout_pulsewidth_us(pwmout_t* obj, long us);

    inline void pwmout_pulsewidth_ms(pwmout_t* obj, long ms) {
        pwmout_pulsewidth_us(obj, ms * 1000);
    }

    inline void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
        pwmout_pulsewidth_us(obj, seconds / 1000000.0f);
    }

}

#endif
