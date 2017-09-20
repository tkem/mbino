/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
 *
 * mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.  See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include "DigitalInOut.h"

#include "platform/mbed_critical.h"

namespace mbino {

    DigitalInOut::DigitalInOut(PinName pin) : _mode(PullDefault), _value(0)
    {
        gpio_init_in_ex(&gpio, pin, _mode);
    }

    DigitalInOut::DigitalInOut(PinName pin, PinDirection direction, PinMode mode, int value)
        : _mode(mode), _value(value)
    {
        if (direction == PIN_INPUT) {
            gpio_init_in_ex(&gpio, pin, _mode);
        } else {
            gpio_init_out_ex(&gpio, pin, _value);
        }
    }

    void DigitalInOut::write(int value)
    {
        core_util_critical_section_enter();
        gpio_write(&gpio, (_value = value));
        _value = value;
        core_util_critical_section_exit();
    }

    void DigitalInOut::mode(PinMode pull)
    {
        core_util_critical_section_enter();
        gpio_mode(&gpio, (_mode = pull));
        core_util_critical_section_exit();
    }

    DigitalInOut& DigitalInOut::operator=(DigitalInOut& rhs)
    {
        core_util_critical_section_enter();
        write(rhs.read());
        core_util_critical_section_exit();
        return *this;
    }

}
