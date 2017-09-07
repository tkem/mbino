#include "mbed.h"

PwmOut pwm(D9);  // change to D11 on Arduino Mega

void setup() {
    pwm.period(4.0f);  // 4 second period
    pwm.write(0.50f);  // 50% duty cycle, relative to period
}

void loop() {
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
