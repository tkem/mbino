#include "mbed.h"

InterruptIn button(BUTTON1);
DigitalOut led(LED1);
DigitalOut flash(LED2);

void flip() {
    led = !led;
}

void setup() {
    // attach the address of the flip function to the rising edge
    button.rise(&flip);
}

void loop() {
    // wait around, interrupts will interrupt this!
    flash = !flash;
    wait(5.0f);
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
