#include "mbed.h"

Timer t;

RawSerial pc(USBTX, USBRX);

void setup() {
    t.start();
    pc.printf("Hello World!\r\n");
    t.stop();
    pc.printf("The time taken was %ld microseconds\r\n", long(t.read_us()));
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
