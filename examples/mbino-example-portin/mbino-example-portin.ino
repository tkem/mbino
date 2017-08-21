// Switch on an LED if any of ATmega168/328 pins 8-12 is high

#include "mbed.h"

PortIn port(PortB, 0x1F);  // D8-D12
DigitalOut led1(LED1);

void setup() {
}

void loop() {
    int pins = port.read();
    if (pins) {
        led1 = 1;
    } else {
        led1 = 0;
    }
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
