#include "mbed.h"

// use fully qualified class name to resolve ambiguities with
// Arduino's own global "SPI" object
mbed::SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK);
DigitalOut cs(D10);

RawSerial pc(USBTX, USBRX);

static const uint8_t READ = 0x03;  // read data from memory array
static const uint8_t WRITE = 0x02; // write data to memory array
static const uint8_t WRDI = 0x04;  // disable write operations
static const uint8_t WREN = 0x06;  // enable write operations
static const uint8_t RDSR = 0x05;  // read STATUS register
static const uint8_t WRSR = 0x01;  // write STATUS register

uint16_t address = 0;  // 0..511

void setup() {
    // Chip must be deselected
    cs = 1;

    // Setup the spi for 8 bit data, high steady state clock, second
    // edge capture, with a 1MHz clock rate
    spi.format(8, 0);

    spi.set_default_write_value(0);
}

void loop() {
    const char tx_buffer[16] = "0123456789ABCDEF";
    char rx_buffer[16];

    pc.printf("write @0x%03x: %.16s\r\n", address, tx_buffer);

    cs = 0;
    wait_ms(1);
    spi.write(WREN);
    spi.write(0);
    wait_ms(1);
    cs = 1;

    wait_ms(1);

    cs = 0;
    wait_ms(1);
    spi.write(WRITE | ((address & 0x100) >> 5));
    spi.write(address & 0xff);
    spi.write(tx_buffer, sizeof tx_buffer, 0, 0);
    wait_ms(1);
    cs = 1;

    wait_ms(1);

    cs = 0;
    wait_ms(1);
    spi.write(READ | ((address & 0x100) >> 5));
    spi.write(address & 0xff);
    spi.write(0, 0, rx_buffer, sizeof rx_buffer);
    wait_ms(1);
    cs = 1;

    pc.printf("read @0x%03x: %.16s\r\n", address, rx_buffer);

    address += 16;
    address %= 512;

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
