#include "mbed.h"

// user-defined standard serial with non-standard baud setting
mbed::Serial pc(STDIO_UART_TX, STDIO_UART_RX, 115200);
// deliberately swap RX and TX pins - comment out to see other use cases
mbed::Serial bogus(SERIAL_RX, SERIAL_TX);

void setup() {
    pc.printf("Hello world!\r\n");
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
