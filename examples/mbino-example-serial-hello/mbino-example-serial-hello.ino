#include "mbed.h"

mbed::Serial pc(USBTX, USBRX);

void setup() {
    pc.printf("Hello World!\r\n");
}

void loop() {
    // echo input back to terminal
    pc.putc(pc.getc() + 1);
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
