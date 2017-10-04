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

#include "platform/FileHandle.h"
#include "platform/mbed_retarget.h"

#if DEVICE_STDIO_MESSAGES
#include "platform/mbed_stdio.h"
#include "hal/serial_api.h"
#endif

#include <stdio.h>

namespace mbino {

    static int put(char c, FILE* fp)
    {
        FileHandle* fh = static_cast<FileHandle*>(fdev_get_udata(fp));
        return fh->write(&c, 1) == 1 ? 0 : -1;
    }

    static int get(FILE* fp)
    {
        unsigned char c;
        FileHandle* fh = static_cast<FileHandle*>(fdev_get_udata(fp));
        switch (fh->read(&c, 1)) {
        case 1:
            return c;
        case 0:
            return _FDEV_EOF;
        default:
            return _FDEV_ERR;
        }
    }

    FILE* mbed_fdopen(FileHandle* fh, const char* mode)
    {
        bool rd = mode[0] == 'r' || mode[1] == '+';
        bool wr = mode[0] == 'w' || mode[1] == '+';
        FILE* fp = fdevopen(wr ? put : 0, rd ? get : 0);
        fdev_set_udata(fp, fh);
        return fp;
    }

    void mbed_set_unbuffered_stream(FILE*)
    {
        // unbuffered by default
    }

}

#if DEVICE_STDIO_MESSAGES

extern int (*mbed_error_vprintf)(const char* format, va_list arg);

static FILE stdio_stream;

static int stdio_put(char c, FILE*)
{
    serial_putc(&stdio_uart, c);
    return 0;
}

static int stdio_get(FILE*)
{
    return serial_getc(&stdio_uart);
}

void mbed_stdio_init(long baudrate)
{
    stdio_uart_init();
    if (baudrate) {
        serial_baud(&stdio_uart, baudrate);
    }
    fdev_setup_stream(&stdio_stream, stdio_put, stdio_get, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &stdio_stream;
    mbed_error_vprintf = vprintf;
}

void mbed_stdio_flush()
{
    // TODO: flush?
}

#endif

#endif
