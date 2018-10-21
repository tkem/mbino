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

#include "MultiFunctionShieldDisplay.h"

/*
  This example uses the popular "Arduino Multi-function Shield".  With
  this shield, the display needs to be updated periodically, and
  buttons, LEDs and the buzzer are active low.  See also

  http://arduinolearning.com/code/multi-function-shield-examples.php
  http://www.cohesivecomputing.co.uk/hackatronics/arduino-multi-function-shield/
  http://forum.hobbycomponents.com/viewtopic.php?t=1618
*/
int main() {
    MultiFunctionShieldDisplay display(D4, D7, D8);

    AnalogIn pot(A0);
    BusIn buttons(A1, A2, A3);
    DigitalOut buzzer(D3, 1);
    PwmOut pwm(D10);
    BusOut leds(D13, D12, D11);
    BusIn header(D5, D6, D9, A5);

    // start updating the display
    display.start();

    // PWM period will be ignored on the Arduino Mega, which uses an
    // 8-bit timer for D10 PWM; you'll only see the LED fade for low
    // pot values.
    pwm.period(1.0f);

    // use internal pull-up resistors on header pins
    header.mode(PullUp);

    float pot_flt = 0.0f;
    unsigned pot_u16 = 0;
    int header_val = -1;

    while (true) {
        // A1: sample and hold
        if (buttons[0]) {
            pot_flt = pot.read();
            pot_u16 = pot.read_u16();
        }
        // A2: dec/hex display
        if (buttons[1]) {
            // Arduino stdio doesn't support floating point formats...
            display.printf("%d.%03u", (int)pot_flt, (unsigned)(pot_flt * 1000) % 1000);
        } else {
            display.printf("%04x", pot_u16);
        }
        // A3: buzzer
        buzzer = buttons[2];
        // D10: pulse width = pot value
        pwm.write(1.0f - pot_flt);
        // D13..D11 = A1..A3
        leds = buttons;
        // show header value when changed
        int value = header.read();
        if (header_val != value) {
            printf("Header: 0x%x\r\n", value);
            header_val = value;
        }
    }
}
