#include "mbed.h"

DigitalOut led1(LED_BUILTIN);

void setup() {
}

void loop() {
    led1 = !led1;
    wait(0.5);
}
