#include "mbed.h"

RawSerial pc(USBTX, USBRX);

void setup() {
    pc.printf("Hello World!\r\n");
}

void loop() {
    int c = pc.getc();
    // FIXME: mbed getc() blocking?
    // pc.putc(pc.getc() + 1); // echo input back to terminal
    if (c >= 0) {
        pc.putc(c + 1); // echo input back to terminal
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