#define MBED_CONF_PLATFORM_STDIO_INIT 1

#include "mbed.h"

// deliberately swap RX and TX pins - comment out to see other use cases
mbed::Serial bogus(SERIAL_RX, SERIAL_TX);

void setup() {
    error("That shouldn't have happened!");
}

void loop() {
    int x = rand() % 10;
    if (x >= 5) {
        error("expected x to be less than 5, but got %d", x);
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
