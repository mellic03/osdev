#include "acpi.hpp"
#include "madt.hpp"

#include <kernel/log.hpp>
#include <kmalloc.h>
#include <string.h>
// #include <driver/ioapic.hpp>
// #include <driver/pic.hpp>
// #include <cpu/cpu.hpp>
// #include <kernel/ioport.hpp>
// #include <kernel/bitmanip.hpp>


void ACPI::init( void *addr )
{
    syslog log("ACPI::init");

    RSDP_t *rsdp = (RSDP_t*)addr;
    RSDT_t *rsdt = nullptr;
    MADT_t *madt = nullptr;

    log("rsdp: 0x%lx", rsdp);
    bool is_v1 = (rsdp->Revision == 0);
    log("%s", (is_v1 ? "is_v1" : "is_v2"));

    if (is_v1)
    {
        uintptr_t rsdt_phys = rsdp->RsdtAddress;
        uintptr_t rsdt_virt = rsdp->RsdtAddress + PMM::hhdm;
        log("rsdt_phys: 0x%lx", rsdt_phys);
        log("rsdt_virt: 0x%lx", rsdt_virt);

        rsdt = (RSDT_t*)rsdt_virt;
        madt = ACPI::findTable<MADT_t>(rsdt, "APIC");
    }

    else
    {
        auto *xsdt = (XSDT_t*)((rsdp->RsdtAddress) + PMM::hhdm);
        log("xsdt: 0x%lx", xsdt);
    }
        
    if (madt)
    {
        ACPI::MADT_init(madt);
    }
}











// ACPI::MADT_t *ACPI::findMADT( RSDT_t *rsdt )
// {
//     int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;

//     for (int i = 0; i < entries; i++)
//     {
//         ACPISDTHeader *h = (ACPISDTHeader *)(rsdt->PointerToOtherSDT[i]);
//         if (!strncmp(h->Signature, "MADT", 4))
//             return (MADT_t*)h;
//     }

//     return nullptr;
// }











