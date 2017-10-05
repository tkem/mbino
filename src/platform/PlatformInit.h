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
#ifndef MBINO_PLATFORM_INIT_H
#define MBINO_PLATFORM_INIT_H

#include "platform/platform.h"
#include "platform/NonCopyable.h"

#ifndef MBED_CONF_PLATFORM_STDIO_BAUD_RATE
#define MBED_CONF_PLATFORM_STDIO_BAUD_RATE
#endif

namespace mbino {

    // mbino extension to retarget stdio before main()
    class PlatformInit : private NonCopyable<PlatformInit> {
        static unsigned counter;

    public:
        PlatformInit() {
            if (counter++ == 0) {
                stdio_init(MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
            }
        }

        ~PlatformInit() {
            if (--counter == 0) {
                stdio_flush();
            }
        }

    private:
        static void stdio_init(long baudrate = 0);
        static void stdio_flush();
    };

}

#if DEVICE_STDIO_MESSAGES
static mbino::PlatformInit _mbed_platform_init;
#endif

#endif
