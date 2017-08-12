#include "mbed.h"

Timer t;

RawSerial pc(USBTX, USBRX);

void setup() {
}

void loop() {
    t.start();
    pc.printf("Hello World!\r\n");
    t.stop();
    pc.printf("Time taken was %ld microseconds\r\n", long(t.read_us()));
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
