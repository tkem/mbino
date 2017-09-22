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
#ifndef MBINO_SERIAL_BASE_H
#define MBINO_SERIAL_BASE_H

#include "platform/platform.h"

#if defined(DEVICE_SERIAL) || defined(DOXYGEN_ONLY)

#include "hal/serial_api.h"
#include "platform/Callback.h"
#include "platform/NonCopyable.h"

namespace mbino {

    class SerialBase : private NonCopyable<SerialBase> {
        serial_t _serial;
        long _baud;

    public:

        enum Parity {
            None = ParityNone,
            Odd = ParityOdd,
            Even = ParityEven,
            Forced1 = ParityForced1,
            Forced0 = ParityForced0
        };

        enum IrqType {
            RxIrq = 0,
            TxIrq,
            IrqCnt
        };

        enum Flow {
            Disabled = FlowControlNone,
            RTS = FlowControlRTS,
            CTS = FlowControlCTS,
            RTSCTS = FlowControlRTSCTS
        };

        // mbino extension: change baudrate type to long
        void baud(long baudrate) {
            serial_baud(&_serial, (_baud = baudrate));
        }

        void format(int bits = 8, Parity parity = SerialBase::None, int stop_bits = 1) {
            serial_format(&_serial, bits, (SerialParity)parity, stop_bits);
        }

        int readable() {
            return serial_readable(&_serial);
        }

        int writeable() {
            return serial_writable(&_serial);
        }

        // mbino extension: pass const reference to func
        void attach(const Callback<void()>& func, IrqType type = RxIrq);

        void send_break();

#if DEVICE_SERIAL_FC
        void set_flow_control(Flow type, PinName flow1 = NC, PinName flow2 = NC);
#endif

#if DEVICE_SERIAL_ASYNCH
#error "Serial anynchronous operation is not suupported."
#endif

    protected:

        // mbino extension: change baudrate type to long
        SerialBase(PinName tx, PinName rx, long baud);

        // FIXME mbino extension: Arduino monitor serial w/o hardware pins
        SerialBase(PinNameMonitorTX, PinNameMonitorRX, long baud);

        ~SerialBase() {
            // mbino extension: serial_free() not called in mbed, but
            // need to detach IRQ handlers...
            serial_free(&_serial);
        }

        int _base_getc() {
            return serial_getc(&_serial);
        }

        int _base_putc(int c) {
            serial_putc(&_serial, c);
            return c;
        }

        // mbino extension
        int _base_puts(const char* s) {
            serial_puts(&_serial, s);
            return 0;
        }

        /* mbino restriction: no lock methods
        virtual void lock();
        virtual void unlock();
        */

    private:
        Callback<void()> _irq[IrqCnt];

        // mbino extension: change id type to intptr_t
        static void _irq_handler(intptr_t id, SerialIrq irq_type);
    };

}

#endif

#endif
