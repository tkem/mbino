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

// Scan for standard 7-bit addresses

int main() {
    I2C i2c(I2C_SDA, I2C_SCL);

    while (true) {
        printf("Scanning for I2C devices...\r\n");

        int nfound = 0;
        for (int address = 0; address != 128; ++address) {
            if (i2c.write(address << 1, 0, 0) == 0) {
                printf("0x%x ACK\r\n", address);
                ++nfound;
            }
        }
        printf("%d devices found\r\n", nfound);
        wait(5);
    }
}
