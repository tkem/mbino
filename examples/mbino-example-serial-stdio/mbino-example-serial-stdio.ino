#define DEVICE_STDIO_MESSAGES 1

#include "mbed.h"

void setup() {
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
