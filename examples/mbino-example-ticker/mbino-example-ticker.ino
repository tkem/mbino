#include "mbed.h"

Ticker flipper;
DigitalOut led(LED1);
DigitalOut flash(LED2);

void flip() {
    led = !led;
}

void setup() {
    flipper.attach(&flip, 2.0); // the address of the function to be attached (flip) and the interval (2 seconds)
}

void loop() {
    // wait around, tickers will interrupt this!
    flash = !flash;
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
