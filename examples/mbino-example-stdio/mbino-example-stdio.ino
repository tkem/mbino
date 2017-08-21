#include "mbed.h"

mbed::Serial pc(USBTX, USBRX);

void setup() {
    // assign standard streams
    stdin = stdout = stderr = pc;
    printf("Hello World!\r\n");
}

void loop() {
    // echo input back to terminal
    putchar(getchar() + 1);
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
