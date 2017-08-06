#include "mbed.h"

DigitalOut led1(LED1);

void setup() {
}

void loop() {
    led1 = !led1;
    wait(0.5);
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
