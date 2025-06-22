#include <arch/acpi.hpp>

#include <kernel/log.hpp>
#include <string.h>
// #include <driver/ioapic.hpp>
// #include <driver/pic.hpp>
// #include <cpu/cpu.hpp>
// #include <arch/io.hpp>
// #include <util/bitmanip.hpp>


void ACPI::init( uintptr_t rsdp_phys, ACPI::Response &res )
{
    syslog log("ACPI::init");

    uintptr_t rsdp_page, rsdt_page, madt_page;

    rsdp_page = idk::align_down(rsdp_phys, PMM::PAGE_SIZE);
    VMM::mapPage(rsdp_page, rsdp_page);

    RSDP_t *rsdp = (RSDP_t*)rsdp_phys;
    RSDT_t *rsdt = nullptr;
    // XSDP_t *xsdp = (XSDP_t*)rsdp_virt;
    // XSDT_t *xsdt = nullptr;
    MADT_t *madt = nullptr;

    log("rsdp phys:     0x%lx", rsdp_phys);
    log("rsdp page:     0x%lx", rsdp_page);
    log("rsdp revision: %u", rsdp->Revision);
    // bool is_v1 = (rsdp->Revision == 0);
    // log("%s", (is_v1 ? "is_v1" : "is_v2"));

    // if (is_v1)
    {
        uintptr_t rsdt_phys = rsdp->RsdtAddress;
        rsdt_page = idk::align_down(rsdp->RsdtAddress, PMM::PAGE_SIZE);
        VMM::mapPage(rsdt_page, rsdt_page);

        rsdt = (RSDT_t*)rsdt_phys;
        log("rsdt phys: 0x%lx", rsdt_phys);
        log("rsdt page: 0x%lx", rsdt_page);
    
        uintptr_t madt_phys = ACPI::findTableRSDT(rsdt, "APIC");
        madt_page = idk::align_down(madt_phys, PMM::PAGE_SIZE);

        log("madt phys: 0x%lx", madt_phys);
        log("madt page: 0x%lx", madt_page);
        madt = (MADT_t*)madt_phys;
    }

    // else
    // {
    //     uintptr_t xsdt_virt = PMM::allocPage() + PMM::hhdm;
    //     // uintptr_t xsdt_virt = xsdp->XsdtAddress;
    //     VMM::mapPage(xsdp->XsdtAddress, xsdt_virt);
    //     xsdt = (XSDT_t*)xsdt_virt;
    //     log("xsdt: 0x%lx", xsdt);
    //     madt = ACPI::findTableXSDT<MADT_t>(xsdt, "APIC");
    // }

    if (madt)
    {
        ACPI::MADT_init(madt, res);
    }

    VMM::unmapPage(rsdt_page);
    VMM::unmapPage(rsdp_page);
    VMM::unmapPage(madt_page);
}

