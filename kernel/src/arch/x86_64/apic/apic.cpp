#include <arch/apic.hpp>
#include <arch/acpi.hpp>
#include <cpu/cpu.hpp>
#include <driver/pic.hpp>
#include <kernel/memory/vmm.hpp>
#include <kernel/interrupt.hpp>
// #include <arch/apic.hpp>

/*
    #define IA32_APIC_BASE_MSR 0x1B
    #define IA32_APIC_BASE_MSR_ENABLE 0x800
    #define IA32_APIC_BASE_MSR_BSP 0x100

    // Possible way of accessing apic_base without MADT (Unsure if stable).
    uintptr_t apic_base = CPU::rdmsr(IA32_APIC_BASE_MSR) & 0xfffff000;
    log("apic_base: 0x%lx", apic_base);
*/

uintptr_t APIC::lapicBase  = 0;
uintptr_t APIC::ioapicBase = 0;
uintptr_t APIC::gsiBase    = 0;


void APIC::init( const ACPI::Response &res )
{
    PIC::disable();

    lapicBase  = res.lapic_base;
    ioapicBase = res.ioapic_base;
    gsiBase    = res.gsi_base;

    VMM::mapPage(lapicBase, lapicBase);
    VMM::mapPage(ioapicBase, ioapicBase);
    VMM::mapPage(gsiBase, gsiBase);

    // LAPIC::init();
    IOAPIC::init();
}


void APIC::write( uint32_t reg, uint32_t data )
{
    *((volatile uint32_t*)(APIC::lapicBase + reg)) = data;
}


uint32_t APIC::read( uint32_t reg )
{
    return *((volatile uint32_t*)(APIC::lapicBase + reg));
}








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

void LAPIC::resetTimer( uint32_t us )
{
    uint8_t  irqno  = IrqNo_PIT;
    uint8_t  isrno  = IntNo_IOAPIC_Base + irqno;
    uint32_t config = isrno | (1 << 17);
    APIC::write(APIC::REG_LVT_TIMER, config);
    APIC::write(APIC::REG_TIMER_INIT, 1000000/us);
}


void LAPIC::init( uint32_t usecs )
{
    auto *cpu = SMP::this_cpu();

    syslog log("LAPIC::init cpu=%lu", cpu->id);
    cpu->m_lapicPeriod.store(usecs);
    log("A");

    APIC::write(APIC::REG_SPURIOUS, APIC::ENABLE | IntNo_Spurious);
    log("Enabled spurious vector with isrno=%u", IntNo_Spurious);

    LAPIC::resetTimer(usecs);
    log("Enabled lapic timer");

    log("Done!");
}


#define ICR_VECTOR(x) (x & 0xFF)
#define ICR_MESSAGE_TYPE_FIXED 0
#define ICR_MESSAGE_TYPE_LOW_PRIORITY (1 << 8)
#define ICR_MESSAGE_TYPE_SMI (2 << 8)
#define ICR_MESSAGE_TYPE_REMOTE_READ (3 << 8)
#define ICR_MESSAGE_TYPE_NMI (4 << 8)
#define ICR_MESSAGE_TYPE_INIT (5 << 8)
#define ICR_MESSAGE_TYPE_STARTUP (6 << 8)
#define ICR_MESSAGE_TYPE_EXTERNAL (7 << 8)

#define ICR_DSH_DEST 0          // Use destination field
#define ICR_DSH_SELF (1 << 18)  // Send to self
#define ICR_DSH_ALL (2 << 18)   // Send to ALL APICs
#define ICR_DSH_OTHER (3 << 18) // Send to all OTHER APICs 


void SendIPI( uint8_t dst_apicid, uint32_t dsh /* Destination Shorthand*/,
              uint32_t type, uint8_t isrno )
{
    uint32_t high = ((uint32_t)dst_apicid) << 24;
    uint32_t low = dsh | type | ICR_VECTOR(isrno);

    APIC::write(APIC::REG_ICR_HI, high);
    APIC::write(APIC::REG_ICR_LO, low);
}

void LAPIC::sendIPI( uint32_t lapic_id, uint8_t isrno )
{
    SendIPI(lapic_id, ICR_DSH_DEST, ICR_MESSAGE_TYPE_FIXED, isrno);
}


void LAPIC::sendEOI()
{
    APIC::write(APIC::REG_EOI, 0);
}