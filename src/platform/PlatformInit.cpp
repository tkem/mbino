/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
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
#include "platform/PlatformInit.h"
#include "platform/platform.h"
#include "platform/FileHandle.h"
#include "platform/mbed_retarget.h"
#include "drivers/UARTSerial.h"

#include <stdio.h>

extern "C" {
    void mbed_sdk_init(void);
}

#if MBINO_CONF_PLATFORM_STDIO

#if DEVICE_SERIAL
extern int stdio_uart_inited;
extern serial_t stdio_uart;

class DirectSerial : public mbed::FileHandle {
public:
    DirectSerial(PinName tx, PinName rx, long baud) {
        if (!stdio_uart_inited) {
            serial_init(&stdio_uart, tx, rx);
            serial_baud(&stdio_uart, baud);
        }
    }

    ssize_t write(const void* buffer, size_t size) {
        const unsigned char* buf = static_cast<const unsigned char*>(buffer);
        const unsigned char* end = buf + size;
        while (buf != end) {
            serial_putc(&stdio_uart, *buf++);
        }
        return size;
    }

    ssize_t read(void* buffer, size_t size) {
        if (size != 0) {
            *static_cast<unsigned char*>(buffer) = serial_getc(&stdio_uart);
            return 1;
        } else {
            return 0;
        }
    }

    off_t seek(off_t offset, int whence = SEEK_SET) {
        return -ESPIPE;
    }

    off_t size() {
        return -EINVAL;
    }

    int isatty() {
        return true;
    }

    int close() {
        return 0;
    }

    short poll(short events) const {
        short revents = 0;
        if ((events & POLLIN) && serial_readable(&stdio_uart)) {
            revents |= POLLIN;
        }
        if ((events & POLLOUT) && serial_writable(&stdio_uart)) {
            revents |= POLLOUT;
        }
        return revents;
    }
};

#endif

class Sink : public mbed::FileHandle {
public:
    ssize_t write(const void* buffer, size_t size) {
        return size;
    }

    ssize_t read(void* buffer, size_t size) {
        if (size != 0) {
            *static_cast<unsigned char*>(buffer) = 0;
            return 1;
        } else {
            return 0;
        }
    }

    off_t seek(off_t offset, int whence = SEEK_SET) {
        return -ESPIPE;
    }

    off_t size() {
        return -EINVAL;
    }

    int isatty() {
        return true;
    }

    int close() {
        return 0;
    }
};

static mbed::FileHandle* default_console() {
#if DEVICE_SERIAL
#if MBED_CONF_PLATFORM_STDIO_BUFFERED_SERIAL
    static mbed::UARTSerial console(STDIO_UART_TX, STDIO_UART_RX, MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
#else
    static DirectSerial console(STDIO_UART_TX, STDIO_UART_RX, MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
#endif
#else
    static Sink console;
#endif
    return &console;
}

static mbed::FileHandle* get_console(int fd)
{
    if (mbed::FileHandle* fh = mbed::mbed_override_console(fd)) {
        return fh;
    } else if (mbed::FileHandle* fh = mbed::mbed_target_override_console(fd)) {
        return fh;
    } else {
        return default_console();
    }
}

static void init_stdio()
{
    if (mbed::FileHandle* fh = get_console(STDIN_FILENO)) {
        stdin = mbed::fdopen(fh, "r");
    }
    if (mbed::FileHandle* fh = get_console(STDOUT_FILENO)) {
        stdout = mbed::fdopen(fh, "w");
    }
    if (mbed::FileHandle* fh = get_console(STDERR_FILENO)) {
        stderr = mbed::fdopen(fh, "w");
    }
}

#endif

bool mbed::PlatformInit::inited = false;

void mbed::PlatformInit::init()
{
    mbed_sdk_init();
#if MBINO_CONF_PLATFORM_STDIO
    init_stdio();
#endif
    inited = true;
}
