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
#include "platform/mbed_wait_api.h"

#include <Arduino.h>

void wait_us(long us)
{
    // "Currently, the largest value that will produce an accurate delay is 16383."
    // https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
    if (us <= 16383) {
        delayMicroseconds(us);
    } else {
        delay((us + 999) / 1000);  // wait for *minimum* us microseconds
    }
}
