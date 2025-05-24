#include <arch/apic.hpp>
#include <arch/acpi.hpp>
#include <driver/pic.hpp>
#include <kernel/memory/vmm.hpp>
// #include <arch/apic.hpp>

/*
    #define IA32_APIC_BASE_MSR 0x1B
    #define IA32_APIC_BASE_MSR_ENABLE 0x800
    #define IA32_APIC_BASE_MSR_BSP 0x100

    // Possible way of accessing apic_base without MADT (Unsure if stable).
    uintptr_t apic_base = CPU::rdmsr(IA32_APIC_BASE_MSR) & 0xfffff000;
    log("apic_base: 0x%lx", apic_base);
*/

void IOAPIC_setRegister( uint32_t idx, uint32_t value );


void APIC::init( const ACPI::Response &res )
{
    PIC::disable();

    LAPIC::init(res);
    IOAPIC::init(res);
    // IOAPIC_setRegister(0, )

}











/*
    The interrupt command register is made of two 32-bit registers; one at 0x300 and 0x310.
    It is used for sending interrupts to different processors.
    The interrupt is issued when 0x300 is written to, but not when 0x310 is written to.
    Thus, to send an interrupt command one should first write to 0x310, then to 0x300.
    At 0x310 there is one field at bits 24-27, which is local APIC ID of the target processor
    (for a physical destination mode).
*/
static uintptr_t g_lapicBase;


void lapic_write_reg( uint32_t reg, uint32_t data )
{
    *((volatile uint32_t*)(g_lapicBase + reg)) = data;
}

uint32_t lapic_read_reg( uint32_t reg )
{
    return *((volatile uint32_t*)(g_lapicBase + reg));
}


void LAPIC::init( const ACPI::Response &res )
{
    syslog log("LAPIC::init");
    g_lapicBase = res.lapic_base;
    VMM::mapPage(g_lapicBase, g_lapicBase);
    // log("g_lapicBase: 0x%lx", g_lapicBase);
    lapic_write_reg(APIC::REG_SPURIOUS, APIC::ENABLE | APIC::SPURIOUS_INT);
    log("Done!");
}





