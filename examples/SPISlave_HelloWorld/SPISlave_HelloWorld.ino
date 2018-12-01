/* mbino Example Program
 * Copyright (c) 2018 Thomas Kemmer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"

int main() {
    SPISlave device(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_SS);
    DigitalOut led1(LED1);

    device.reply(0xff);              // first response

    while (true) {
        if(device.receive()) {       // if byte was received
            int v = device.read();   // read byte from master
            device.reply(v);         // echo back in next reply
            led1 = !led1;            // flash LED
        }
    }
}
