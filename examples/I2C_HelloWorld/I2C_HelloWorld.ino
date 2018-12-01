/* mbed Example Program
 * Copyright (c) 2006-2014 ARM Limited
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

// Read temperature from LM75BD

I2C i2c(I2C_SDA, I2C_SCL);

const int addr7bit = 0x48;      // 7 bit I2C address
const int addr8bit = 0x48 << 1; // 8bit I2C address, 0x90

int main() {
    unsigned char cmd[2];
    while (1) {
        cmd[0] = 0x01;
        cmd[1] = 0x00;
        i2c.write(addr8bit, cmd, 2);

        wait(0.5);

        cmd[0] = 0x00;
        i2c.write(addr8bit, cmd, 1);
        i2c.read( addr8bit, cmd, 2);

        float tmp = (float((cmd[0]<<8)|cmd[1]) / 256.0);
#ifdef __WITH_AVRLIBC__
        // AVR libc in Arduino does not support floating point conversions
        printf("Temp = %d.%02u\n", int(tmp), unsigned(tmp * 100.0f) % 100);
#else
        printf("Temp = %.2f\n", tmp);
#endif
    }
}
