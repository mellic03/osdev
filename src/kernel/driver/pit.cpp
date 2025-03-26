#include "pit.hpp"
#include "serial.hpp"
#include <algorithm>


// I/O Ports
#define PIT_COUNTER0                    0x40
#define PIT_CMD                         0x43

// BCD
#define CMD_BINARY                      0x00    // Use Binary counter values
#define CMD_BCD                         0x01    // Use Binary Coded Decimal counter values

// Mode
#define CMD_MODE0                       0x00    // Interrupt on Terminal Count
#define CMD_MODE1                       0x02    // Hardware Retriggerable One-Shot
#define CMD_MODE2                       0x04    // Rate Generator
#define CMD_MODE3                       0x06    // Square Wave
#define CMD_MODE4                       0x08    // Software Trigerred Strobe
#define CMD_MODE5                       0x0a    // Hardware Trigerred Strobe

// Read/Write
#define CMD_LATCH                       0x00
#define CMD_RW_LOW                      0x10    // Least Significant Byte
#define CMD_RW_HI                       0x20    // Most Significant Byte
#define CMD_RW_BOTH                     0x30    // Least followed by Most Significant Byte

// Counter Select
#define CMD_COUNTER0                    0x00
#define CMD_COUNTER1                    0x40
#define CMD_COUNTER2                    0x80
#define CMD_READBACK                    0xc0



uint16_t idk::PIT_HERTZ = 500;

void
idk::PIT_init()
{
    PIT_set_ms(10);
    IO::outb(PIT_CMD, CMD_MODE3 | CMD_RW_BOTH | CMD_COUNTER0);  // Set mode 2
    PIT_reload();
}


void
idk::PIT_reload()
{
    uint16_t reload_value = PIT_FREQUENCY / PIT_HERTZ;
    IO::outb(PIT_COUNTER0, reload_value & 0xFF);
    IO::outb(PIT_COUNTER0, (reload_value >> 8) & 0xFF);
}




void
idk::PIT_set_hz( uint16_t hz )
{
    PIT_HERTZ = hz;
}


void
idk::PIT_set_ms( uint16_t ms )
{
    constexpr uint32_t re = 1000000 / (1000 * 2);
    PIT_set_hz(1000000 / (1000 * ms));

}






uint16_t
idk::PIT_read()
{
    IO::outb(PIT_CMD, CMD_LATCH | CMD_COUNTER0); // Latch the current counter value
    uint8_t low = IO::inb(PIT_COUNTER0);        // Read the low byte
    uint8_t high = IO::inb(PIT_COUNTER0);       // Read the high byte
    return (uint16_t(high) << 8) | low;         // Combine high and low bytes
}

bool
idk::PIT_edge()
{
    static uint16_t prev = 0;
    uint16_t curr = PIT_read();

    bool edge = (curr > prev);
    prev = curr;

    return edge;
}



