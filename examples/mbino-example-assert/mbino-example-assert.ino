#include "mbed.h"

DigitalOut led(LED1, 1);
DigitalIn pullup(D2, PullUp);

void setup() {
}

void loop() {
    MBED_ASSERT(led == 1);
    MBED_ASSERT(pullup == 1);
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
