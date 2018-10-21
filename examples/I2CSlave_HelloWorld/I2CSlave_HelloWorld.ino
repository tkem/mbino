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
#include <mbed.h>

int main() {
    I2CSlave slave(I2C_SDA, I2C_SCL);
    const char msg[] = "Hello Master!";
    const int addr7bit = 0x42;      // 7-bit I2C address
    const int addr8bit = 0x42 << 1; // 8-bit I2C address

    slave.address(addr8bit);

    while (true) {
        char buf[8] = { 0 };
        int i = slave.receive(), n;
        switch (i) {
        case I2CSlave::ReadAddressed:
            n = slave.write(msg, sizeof msg);
            printf("ReadAddressed: %d '%s'\n", n, msg);
            break;
        case I2CSlave::WriteGeneral:
            n = slave.read(buf, sizeof buf);
            printf("WriteGeneral: %d '%.8s'\n", n, buf);
            break;
        case I2CSlave::WriteAddressed:
            n = slave.read(buf, sizeof buf);
            printf("WriteAddressed: %d '%.8s'\n", n, buf);
            break;
        }
    }
}
