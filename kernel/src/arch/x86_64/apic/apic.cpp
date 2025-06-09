#include <arch/apic.hpp>
#include <arch/acpi.hpp>
#include <cpu/cpu.hpp>
#include <driver/pic.hpp>
#include <kernel/memory/pmm.hpp>
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

    auto p0 = idk::align_down(lapicBase, PMM::PAGE_SIZE);
    auto p1 = idk::align_down(ioapicBase, PMM::PAGE_SIZE);
    auto p2 = idk::align_down(gsiBase, PMM::PAGE_SIZE);

    lapicBase  += PMM::hhdm;
    ioapicBase += PMM::hhdm;
    gsiBase    += PMM::hhdm;
    auto v0 = idk::align_down(lapicBase, PMM::PAGE_SIZE);
    auto v1 = idk::align_down(ioapicBase, PMM::PAGE_SIZE);
    auto v2 = idk::align_down(gsiBase, PMM::PAGE_SIZE);

    VMM::mapPage(p0, v0);
    VMM::mapPage(p1, v1);
    VMM::mapPage(p2, v2);

    syslog log("APIC::init");
    log("lapicBase:  0x%lx", lapicBase);
    log("ioapicBase: 0x%lx", ioapicBase);
    log("gsiBase:    0x%lx", gsiBase);
    // lapicBase  = PMM::allocPages(4) + PMM::hhdm;
    // ioapicBase = PMM::allocPages(4) + PMM::hhdm;
    // gsiBase    = PMM::allocPages(4) + PMM::hhdm;

    // VMM::mapRange(res.lapic_base, lapicBase, 4);
    // VMM::mapRange(res.ioapic_base, ioapicBase, 4);
    // VMM::mapRange(res.gsi_base, gsiBase, 4);

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


