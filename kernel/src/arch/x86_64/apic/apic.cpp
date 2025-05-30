#include <arch/apic.hpp>
#include <arch/acpi.hpp>
#include <cpu/cpu.hpp>
#include <driver/pic.hpp>
#include <kernel/memory/vmm.hpp>
#include <sys/interrupt.hpp>


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


