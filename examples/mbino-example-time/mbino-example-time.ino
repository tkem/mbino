#include "mbed.h"

mbed::Serial pc(USBTX, USBRX);

void setup() {
    set_time(1507134010);  // Set RTC time to Wed, 04 Oct 2017 16:20:10
}

void loop() {
    time_t seconds = time(NULL);
    pc.printf("Time as seconds since January 1, 1970 = %ld\r\n", seconds);
    pc.printf("Time as a basic string = %s\r\n", ctime(&seconds));

    char buffer[32];
    strftime(buffer, 32, "%I:%M %p", localtime(&seconds));
    pc.printf("Time as a custom formatted string = %s\r\n", buffer);

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
