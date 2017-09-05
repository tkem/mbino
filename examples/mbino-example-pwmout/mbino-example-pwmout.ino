#include "mbed.h"

PwmOut pwm(PWM_OUT);  // pin 0 on ATtiny, pin 10 otherwise

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
