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

#include <Arduino.h>

namespace mbino {

    void pwmout_free(pwmout_t* obj)
    {
        digitalWrite(obj->pin, obj->value >= 128);
    }

    void pwmout_write_u8(pwmout_t* obj, uint8_t value)
    {
        uint8_t sreg = SREG;
        analogWrite(obj->pin, obj->value);
        obj->value = value;
        SREG = sreg;
    }

/* Not implemented (yet)!

    void pwmout_period_us(pwmout_t *obj, long us);

    void pwmout_pulsewidth_us(pwmout_t *obj, long us);
*/

}
