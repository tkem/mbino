// Toggle ATmega168/328 pins 8-12

#include "mbed.h"

PortOut port(PortB, 0x1F);  // D8-D12

void setup() {
}

void loop() {
    port = ~0;
    wait(1);
    port = 0;
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
