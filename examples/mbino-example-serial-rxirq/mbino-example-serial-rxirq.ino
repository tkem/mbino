#include "mbed.h"

DigitalOut led1(LED1);
//DigitalOut led2(LED2);
mbed::Serial pc(USBTX, USBRX);

void callback() {
    pc.printf("%c\n", pc.getc());
    //led2 = !led2;
}

void setup() {
    pc.attach(callback);
}

void loop() {
    led1 = !led1;
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
