#include "mbed.h"

AnalogIn ain(A0);
PwmOut pwm(PWM_OUT);

void setup() {
}

void loop() {
    pwm.write(ain.read());
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
