#include "mbed.h"

mbed::Serial pc(USBTX, USBRX);

Timer timer;

void setup() {
    Timer t;
    t.start();
    pc.printf("Hello World!\r\n");
    t.stop();
    pc.printf("Time taken was %ld microseconds\r\n", long(t.read_us()));
    timer.start();
}

void loop() {
    long t = timer.read();
    long h = t / 3600;
    unsigned m = t / 60 % 60;
    unsigned s = t % 60;
    pc.printf("Time since start: %ld:%02u:%02u\r\n", h, m, s);
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
