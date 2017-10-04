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
#ifndef MBINO_MBED_STDIO_H
#define MBINO_MBED_STDIO_H

#include "platform/platform.h"
#include "platform/NonCopyable.h"

#if DEVICE_STDIO_MESSAGES

#ifdef __cplusplus
extern "C"{
#endif

// mbino extension: initialize stdio streams
void mbed_stdio_init(long baudrate);

// mbino extension: flush stdio streams
void mbed_stdio_flush(void);

#ifdef __cplusplus
}

namespace mbino {

    class PlatformStdioInit : private NonCopyable<PlatformStdioInit> {
        static unsigned counter;

    public:
        PlatformStdioInit() {
            if (counter++ == 0) {
#if MBED_CONF_PLATFORM_STDIO_BAUD_RATE
                mbed_stdio_init(MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
#else
                mbed_stdio_init(0);
#endif
            }
        }

        ~PlatformStdioInit() {
            if (--counter == 0) {
#if MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT
                mbed_stdio_flush();
#endif
            }
        }
    };

}

#if MBED_CONF_PLATFORM_STDIO_INIT
static mbino::PlatformStdioInit _mbed_stdio_init;
#endif

#endif

#endif

#endif
