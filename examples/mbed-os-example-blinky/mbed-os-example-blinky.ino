#include "mbed.h"

DigitalOut led1(LED1);

int main() {
    while (true) {
        led1 = !led1;
        wait(0.5);
    }
}
