#include <arch/x86_64/arch/apic.hpp>
// #include <arch/apic.hpp>

/*
    #define IA32_APIC_BASE_MSR 0x1B
    #define IA32_APIC_BASE_MSR_ENABLE 0x800
    #define IA32_APIC_BASE_MSR_BSP 0x100

    // Possible way of accessing apic_base without MADT (Unsure if stable).
    uintptr_t apic_base = CPU::rdmsr(IA32_APIC_BASE_MSR) & 0xfffff000;
    log("apic_base: 0x%lx", apic_base);
*/

static uint32_t *iobase;
// static uint32_t localbase;

void IOAPIC_setRegister( uint32_t idx, uint32_t value );

void APIC::init( uint64_t ioapic_base, uint64_t )
{
    iobase = (uint32_t*)ioapic_base;

    // IOAPIC_setRegister(0, )

}






// https://wiki.osdev.org/IOAPIC#Programming_the_I/O_APIC
#define IOAPICID          0x00
#define IOAPICVER         0x01
#define IOAPICARB         0x02
#define IOAPICREDTBL(n)   (0x10 + 2 * n) // lower-32bits (add +1 for upper 32-bits)
// ------------------------------------------------------------------------------------------------

// static void IoApicOut(u8 *base, u8 reg, u32 val)
// {
//     MmioWrite32(base + IOREGSEL, reg);
//     MmioWrite32(base + IOWIN, val);
// }
// void IoApicSetEntry(u8 *base, u8 index, u64 data)
// {
//     IoApicOut(base, IOREDTBL + index * 2, (u32)data);
//     IoApicOut(base, IOREDTBL + index * 2 + 1, (u32)(data >> 32));
// }


uint32_t IOAPIC::read( uint32_t reg )
{
    uint32_t volatile *addr = (uint32_t volatile *)iobase;
    addr[0] = (reg & 0xff);
    return addr[4];
}

void IOAPIC::write( uint32_t reg, uint32_t value )
{
    uint32_t volatile *addr = (uint32_t volatile *)iobase;
    addr[0] = (reg & 0xff);
    addr[4] = value;
}

void IOAPIC_setRegister( uint32_t idx, uint32_t value )
{
    IOAPIC::write(0x10 + 2*idx+0, value);
    IOAPIC::write(0x10 + 2*idx+1, uint32_t(uint64_t(value) >> 32));
}








/*
    The interrupt command register is made of two 32-bit registers; one at 0x300 and 0x310.
    It is used for sending interrupts to different processors.
    The interrupt is issued when 0x300 is written to, but not when 0x310 is written to.
    Thus, to send an interrupt command one should first write to 0x310, then to 0x300.
    At 0x310 there is one field at bits 24-27, which is local APIC ID of the target processor
    (for a physical destination mode).
*/
void LAPIC::init()
{
    
}

