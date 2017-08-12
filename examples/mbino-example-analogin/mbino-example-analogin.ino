#include "mbed.h"

AnalogIn ain(A0);
DigitalOut dout(LED1);
RawSerial pc(USBTX, USBRX);

void setup() {
}

void loop() {
    // test the voltage on the initialized analog pin and if greater
    // than 0.3 * VCC set the digital pin to a logic 1 otherwise a
    // logic 0
    if (ain > 0.3f) {
        dout = 1;
    } else {
        dout = 0;
    }

    // print the percentage and 16 bit normalized values; note that
    // floating point values cannot be use with printf(), so the value
    // needs to be converted to an integer percentage
    pc.printf("percentage: %d\r\n", int(ain.read() * 100));
    pc.printf("normalized: 0x%04X\r\n", ain.read_u16());
    wait(0.2f);
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
