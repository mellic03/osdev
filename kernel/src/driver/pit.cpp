#include <driver/pit.hpp>
#include <driver/pic.hpp>
#include <arch/io.hpp>
#include <kernel/interrupt.hpp>
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


// // 500hz  == 2ms
// // 1000hz == 1ms
// // 2000hz == 0.5ms

uint16_t PIT::HERTZ         = 500;
uint32_t PIT::CurrFrequency = 500;
uint64_t PIT::MicroSeconds  = 2 * 1000; // 2 * 1000 milliseconds


void PIT::init( uint64_t us )
{
    PIT::MicroSeconds  = us; // Hz = 1000000 / (1000*ms) == 1000000 / (1*us)
    PIT::CurrFrequency = 1000000 / PIT::MicroSeconds;

    uint32_t divisor = PIT::FREQUENCY / PIT::CurrFrequency;
    uint8_t lo = (uint8_t)(divisor & 0xFF);
    uint8_t hi = (uint8_t)((divisor>>8) & 0xFF);

    IO::outb(PIT_CMD, CMD_MODE3 | CMD_RW_BOTH | CMD_COUNTER0);
    IO::outb(PIT_COUNTER0, lo);
    IO::outb(PIT_COUNTER0, hi);
}


uint32_t PIT::sleep( uint64_t ms ) 
{
    uint32_t ticks = 0;

    while (ticks < (PIT::CurrFrequency / 1000 * ms))
    {
        ticks++;
        __asm__ ("hlt");
    }
    return ticks;
}






