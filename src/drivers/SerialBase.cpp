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
#include "drivers/RawSerial.h"

#ifdef DEVICE_SERIAL

#include "platform/mbed_critical.h"
#include "platform/mbed_wait_api.h"

namespace mbino {

    SerialBase::SerialBase(PinName tx, PinName rx, long baud) : _baud(baud)
    {
        serial_init(&_serial, tx, rx);
        serial_baud(&_serial, _baud);
        serial_irq_handler(&_serial, SerialBase::_irq_handler, (intptr_t)this);
    }

    void SerialBase::attach(const Callback<void()>& func, IrqType type)
    {
        core_util_critical_section_enter();
        if (func) {
            _irq[type] = func;
            serial_irq_set(&_serial, (SerialIrq)type, 1);
        } else {
            _irq[type] = 0;
            serial_irq_set(&_serial, (SerialIrq)type, 0);
        }
        core_util_critical_section_exit();
    }

    void SerialBase::send_break()
    {
        // Wait for 1.5 frames before clearing the break condition.
        // One bit time (in us) = 1000000/_baud
        // Twelve bits: 12000000/baud delay
        // 1.5 frames: 18000000/baud delay
        serial_break_set(&_serial);
        wait_us(18000000 / _baud);
        serial_break_clear(&_serial);
    }

#if DEVICE_SERIAL_FC
    void SerialBase::set_flow_control(Flow type, PinName flow1, PinName flow2)
    {
        FlowControl flow_type = (FlowControl)type;
        switch(type) {
        case RTS:
            serial_set_flow_control(&_serial, flow_type, flow1, NC);
            break;
        case CTS:
            serial_set_flow_control(&_serial, flow_type, NC, flow1);
            break;
        case RTSCTS:
        case Disabled:
            serial_set_flow_control(&_serial, flow_type, flow1, flow2);
            break;
        default:
            break;
        }
    }
#endif

    void SerialBase::_irq_handler(intptr_t id, SerialIrq irq_type)
    {
        SerialBase *handler = reinterpret_cast<SerialBase*>(id);
        if (handler->_irq[irq_type]) {
            handler->_irq[irq_type]();
        }
    }

}

#endif
