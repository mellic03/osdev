#include "acpi.hpp"
#include "madt.hpp"

#include <kernel/log.hpp>
#include <kmalloc.h>
#include <string.h>
// #include <driver/ioapic.hpp>
// #include <driver/pic.hpp>
// #include <cpu/cpu.hpp>
// #include <arch/io.hpp>
// #include <kernel/bitmanip.hpp>

#define rcast reinterpret_cast


void ACPI::init( uintptr_t phys, ACPI::Response &res )
{
    syslog log("ACPI::init");

    VMM::mapPage(phys, phys); // MUCH easier if identity mapped 
    RSDP_t *rsdp = (RSDP_t*)phys;
    RSDT_t *rsdt = nullptr;
    MADT_t *madt = nullptr;

    log("rsdp: 0x%lx", rsdp);
    bool is_v1 = (rsdp->Revision == 0);
    log("%s", (is_v1 ? "is_v1" : "is_v2"));

    if (is_v1)
    {
        uintptr_t rsdt_addr = rsdp->RsdtAddress;
        VMM::mapPage(rsdt_addr, rsdt_addr);
        rsdt = (RSDT_t*)rsdt_addr;
        log("rsdt: 0x%lx", rsdt);
        madt = ACPI::findTableRSDT<MADT_t>(rsdt, "APIC");
    }

    else
    {
        uintptr_t xsdt_addr = rsdp->RsdtAddress;
        auto *xsdt = (XSDT_t*)xsdt_addr;
        log("xsdt: 0x%lx", xsdt);
        madt = ACPI::findTableRSDT<MADT_t>(rsdt, "APIC");
    }
        
    if (madt)
    {
        ACPI::MADT_init(madt, res);
    }

}

