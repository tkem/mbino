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
#include "PortInOut.h"

#ifdef DEVICE_PORTINOUT

#include "platform/mbed_critical.h"

namespace mbino {

    PortInOut::PortInOut(PortName port, int mask) : _mode(PullDefault), _value(0)
    {
        port_init_in_ex(&_port, port, mask, _mode);
    }

    void PortInOut::write(int value)
    {
        core_util_critical_section_enter();
        port_write(&_port, (_value = value));
        core_util_critical_section_exit();
    }

    void PortInOut::mode(PinMode pull)
    {
        core_util_critical_section_enter();
        port_mode(&_port, (_mode = pull));
        core_util_critical_section_exit();
    }

    PortInOut& PortInOut::operator=(PortInOut& rhs)
    {
        core_util_critical_section_enter();
        write(rhs.read());
        core_util_critical_section_exit();
        return *this;
    }

}

#endif
