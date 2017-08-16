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
#ifndef MBINO_PWM_OUT_H
#define MBINO_PWM_OUT_H

#include "platform/platform.h"

#include "hal/pwmout_api.h"

namespace mbino {

    class PwmOut {
    public:

        PwmOut(PinName pin) {
            pwmout_init(&_pwm, pin);
        }

        ~PwmOut() {
            pwmout_free(&_pwm);
        }

        void write(float value) {
            pwmout_write(&_pwm, value);
        }

        float read() {
            return pwmout_read(&_pwm);
        }

        void period(float seconds) {
            pwmout_period(&_pwm, seconds);
        }

        void period_ms(long ms) {
            pwmout_period_ms(&_pwm, ms);
        }

        void period_us(long us) {
            pwmout_period_us(&_pwm, us);
        }

        void pulsewidth(float seconds) {
            pwmout_pulsewidth(&_pwm, seconds);
        }

        void pulsewidth_ms(long ms) {
            pwmout_pulsewidth_ms(&_pwm, ms);
        }

        void pulsewidth_us(long us) {
            pwmout_pulsewidth_us(&_pwm, us);
        }

        PwmOut& operator=(float value) {
            write(value);
            return *this;
        }

        PwmOut& operator=(PwmOut& rhs);

        operator float() {
            return read();
        }

    protected:
        pwmout_t _pwm;
    };

}

#endif
