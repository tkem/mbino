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
#include "platform/PlatformInit.h"

// TODO: alternative implementation based on serial_api/retarget alone
#include "drivers/Serial.h"

#include <stdio.h>

namespace mbino {

    unsigned PlatformInit::counter = 0;

#if DEVICE_STDIO_MESSAGES
    void PlatformInit::stdio_init(long baudrate)
    {
        static Serial stdio(STDIO_UART_TX, STDIO_UART_RX);
        if (baudrate != 0) {
            stdio.baud(baudrate);
        }
        stdin = stdout = stderr = stdio;
    }

    void PlatformInit::stdio_flush()
    {
        fflush(stdout);
        fflush(stderr);
    }
#endif

}
