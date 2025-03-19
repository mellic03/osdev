#include <kdriver/pit.hpp>
#include <kdriver/serial.hpp>
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




void
idk::PIT_init()
{
    IO::outb(PIT_CMD, CMD_MODE2 | CMD_RW_BOTH | CMD_COUNTER0);  // Set mode 2
    IO::outb(PIT_COUNTER0, PIT_HERTZ & 0xFF);                   // Send low byte
    IO::outb(PIT_COUNTER0, (PIT_HERTZ >> 8) & 0xFF);            // Send high byte
}

uint32_t
idk::PIT_read()
{
    IO::outb(PIT_CMD, CMD_LATCH | CMD_COUNTER0); // Latch the current counter value
    uint8_t low = IO::inb(PIT_COUNTER0);        // Read the low byte
    uint8_t high = IO::inb(PIT_COUNTER0);       // Read the high byte
    return (high << 8) | low;                   // Combine high and low bytes
}

bool
idk::PIT_edge()
{
    static uint32_t lastValue = 0;
    uint32_t currentValue = PIT_read();

    // Check if the counter has wrapped around
    bool wrapped = currentValue > lastValue;
    lastValue = currentValue;
    return wrapped;
}



