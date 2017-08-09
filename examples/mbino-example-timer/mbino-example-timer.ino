#include "mbed.h"

RawSerial pc(USBTX, USBRX);

Timer t;

void setup() {
    t.start();
    pc.printf("Hello World!\r\n");
    t.stop();
    pc.printf("The time taken was %d microseconds\r\n", int(t.read_us()));
}

void loop() {
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
