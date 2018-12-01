/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2018 Thomas Kemmer
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

#include <Arduino.h>

/*
  Main entry point of a "classic" Arduino application ("sketch").

  Note that some platforms provide a slightly different main(), or do
  not even use main() at all (ESP8266?), so this belongs in target.
  This should also go into its own file, otherwise it might conflict
  with any user-defined main() att link time.
*/
int main()
{
    setup();
    for (;;) {
        loop();
        if (serialEventRun) {
            serialEventRun();
        }
    }
    return 0;
}

#endif
