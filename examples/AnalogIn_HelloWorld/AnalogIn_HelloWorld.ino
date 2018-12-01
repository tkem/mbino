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

// Initialize a pins to perform analog input and digital output fucntions
AnalogIn   ain(A0);
DigitalOut dout(LED1);

int main(void)
{
    while (1) {
        // test the voltage on the initialized analog pin
        //  and if greater than 0.3 * VCC set the digital pin
        //  to a logic 1 otherwise a logic 0
        if(ain > 0.3f) {
            dout = 1;
        } else {
            dout = 0;
        }

        // print the percentage and 16 bit normalized values
#ifdef __WITH_AVRLIBC__
        // AVR libc in Arduino does not support floating point conversions
        unsigned long v = ain.read() * 100000.0f;
        printf("percentage: %3lu.%03lu%%\n", v / 1000UL, v % 1000UL);
#else
        printf("percentage: %3.3f%%\n", ain.read()*100.0f);
#endif
        printf("normalized: 0x%04X \n", ain.read_u16());
        wait(0.2f);
    }
}
