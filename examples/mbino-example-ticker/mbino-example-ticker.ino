#include "mbed.h"

Ticker flipper;

DigitalOut led(LED_BUILTIN);

void flip() {
    led = !led;
}

void setup() {
    led = 1;
    flipper.attach(&flip, 2.0);
}

void loop() {
    wait(1);
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
