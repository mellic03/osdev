#include <arch/apic.hpp>
#include <driver/pit.hpp>
#include <driver/pic.hpp>
#include <cpu/cpu.hpp>
#include <sys/interrupt.hpp>


/*
    The interrupt command register is made of two 32-bit registers; one at 0x300 and 0x310.
    It is used for sending interrupts to different processors.
    The interrupt is issued when 0x300 is written to, but not when 0x310 is written to.
    Thus, to send an interrupt command one should first write to 0x310, then to 0x300.
    At 0x310 there is one field at bits 24-27, which is local APIC ID of the target processor
    (for a physical destination mode).
*/

enum APIC_TIMER_DIVIDE_CONFIG
{
    DIVIDE_BY_1   = 0b1011,
    DIVIDE_BY_2   = 0b0000,
    DIVIDE_BY_4   = 0b0001,
    DIVIDE_BY_8   = 0b0010,
    DIVIDE_BY_16  = 0b0011,
    DIVIDE_BY_32  = 0b1000,
    DIVIDE_BY_64  = 0b1001,
    DIVIDE_BY_128 = 0b1010,
};


// void lapic_init_timer( uint8_t isrno, uint32_t us )
// {
//     uint32_t config = isrno | (1 << 17);
//     APIC::write(APIC::REG_LVT_TIMER, config);
//     APIC::write(APIC::REG_TIMER_INIT, 1000000/us);
// }
#include <kprintf.hpp>

void LAPIC::startTimer()
{
    APIC::write(APIC::REG_TIMER_DIV, 0x03);
    // PIT::init(1000000); // sleep for 10ms
    APIC::write(APIC::REG_TIMER_INIT, 0xFFFFFFFF);
    // PIT::sleep(10); // sleep
    APIC::write(APIC::REG_LVT_TIMER, (1<<16)); // stop apic timer

    // Now we know how often the APIC timer has ticked in 10ms
    uint32_t ticksIn10ms = 0xFFFFFFFF - APIC::read(APIC::REG_TIMER_CURR);
    // kprintf("ticksIn10ms: %u\n", ticksIn10ms);


    // Start timer as periodic on IRQ 0, divider 16,
    // with the number of ticks we counted
    uint8_t irqno = IrqNo_PIT;
    uint8_t isrno = IntNo_IOAPIC_Base + irqno;

    auto *cpu = SMP::this_cpu();
    cpu->m_lapicPeriod = ticksIn10ms;

    APIC::write(APIC::REG_LVT_TIMER, isrno | APIC::TIMER_PERIODIC);
    APIC::write(APIC::REG_TIMER_DIV, 0x03);
    APIC::write(APIC::REG_TIMER_INIT, ticksIn10ms);
}


void LAPIC::resetTimer( uint32_t us )
{
    uint8_t irqno = IrqNo_PIT;
    uint8_t isrno = IntNo_IOAPIC_Base + irqno;
    APIC::write(APIC::REG_LVT_TIMER, isrno | APIC::TIMER_PERIODIC);
    APIC::write(APIC::REG_TIMER_INIT, 1000000/us);
}


void LAPIC::init()
{
    APIC::write(APIC::REG_SPURIOUS, APIC::ENABLE | IntNo_Spurious);
    LAPIC::startTimer();
    // LAPIC::resetTimer(usecs);
}


void LAPIC::sendIPI( uint32_t dstid, uint8_t isrno, uint32_t dsh, uint32_t type )
{
    uint32_t high = ((uint32_t)dstid) << 24;
    uint32_t low = dsh | type | ICR_VECTOR(isrno);

    APIC::write(APIC::REG_ICR_HI, high);
    APIC::write(APIC::REG_ICR_LO, low);
}


void LAPIC::sendEOI()
{
    APIC::write(APIC::REG_EOI, 0);
}