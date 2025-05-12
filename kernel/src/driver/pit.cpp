#include <driver/pit.hpp>
#include <driver/pic.hpp>
#include <kernel/ioport.hpp>
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


// 500hz  == 2ms
// 1000hz == 1ms
// 2000hz == 0.5ms

uint16_t PIT::HERTZ = 2000;
uint32_t PIT::CurrFrequency = 2000;


void PIT::init( uint32_t new_frequency )
{
    uint32_t divisor = PIT::FREQUENCY / new_frequency;
    PIT::CurrFrequency = new_frequency;
    uint8_t lo = (uint8_t)(divisor & 0xFF);
    uint8_t hi = (uint8_t)((divisor>>8) & 0xFF);

    IO::outb(PIT_CMD, CMD_MODE3 | CMD_RW_BOTH | CMD_COUNTER0);
    IO::outb(PIT_COUNTER0, lo);
    IO::outb(PIT_COUNTER0, hi);
    PIC::setmask(IrqNo_PIT);
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



// uint16_t PIT::read()
// {
//     IO::outb(PIT_CMD, CMD_LATCH | CMD_COUNTER0); // Latch the current counter value
//     uint8_t low = IO::inb(PIT_COUNTER0);        // Read the low byte
//     uint8_t high = IO::inb(PIT_COUNTER0);       // Read the high byte
//     return (uint16_t(high) << 8) | low;         // Combine high and low bytes
// }

// bool PIT::edge()
// {
//     static uint16_t prev = 0;
//     uint16_t curr = PIT::read();

//     bool edge = (curr > prev);
//     prev = curr;

//     return edge;
// }




// #include <cpu/scheduler.hpp>
// #include <kernel/interrupt.hpp>
// #include <kernel/clock.hpp>
// #include <kthread.hpp>


// static void PIT_IrqHandler( intframe_t *frame )
// {
//     kclock::detail::tick(hwdi_PIT::timer_ms);
//     ThreadScheduler::scheduleISR(frame);
//     PIT::reload();
// }


// hwdi_PIT::hwdi_PIT( uint16_t ms )
// :   hwDriverInterface("PIT Controller")
// {
//     hwdi_PIT::timer_ms = ms;
//     PIT::init();
//     PIT::set_ms(hwdi_PIT::timer_ms);

//     this->irqno    = 0;
//     this->handler  = PIT_IrqHandler;
//     this->entry    = nullptr;
// }


// void
// hwdi_PIT::loadIrqHandler()
// {
//     CPU::installIRQ(this->irqno, this->handler);
//     PIC::unmask(this->irqno);
// }





