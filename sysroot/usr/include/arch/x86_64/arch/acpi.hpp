#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/boot_limine.hpp>
#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>
#include <util/bitmanip.hpp>
#include <kernel/log.hpp>

namespace ACPI
{
    struct Response;

    struct RSDP_t;
    struct XSDP_t;

    struct ACPISDTHeader;
    struct RSDT_t;
    struct XSDT_t;

    struct MADT_t;
    void MADT_init( MADT_t*, ACPI::Response& );

    struct MADT_header_t;
    struct MADT_lapic_t;
    struct MADT_ioapic_t;
    struct MADT_ioapic_iso_t;
}



struct ACPI::RSDP_t
{
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;
} __attribute__ ((packed));


struct ACPI::XSDP_t
{
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;
   
    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExtendedChecksum;
    uint8_t reserved[3];
} __attribute__ ((packed));


struct ACPI::ACPISDTHeader
{
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
} __attribute__ ((packed));


struct ACPI::RSDT_t
{
    ACPISDTHeader header;
    uint32_t PointerToOtherSDT[1]; // [(h.Length - sizeof(h)) / 4];
} __attribute__ ((packed));

struct ACPI::XSDT_t
{
    ACPISDTHeader header;
    uint64_t PointerToOtherSDT[1]; // [(h.Length - sizeof(h)) / 8];
} __attribute__ ((packed));



struct ACPI::MADT_t
{
    ACPISDTHeader header;
    uint32_t lapic_addr;
    uint32_t flags;
    uint8_t  records[2];
} __attribute__ ((packed));


struct ACPI::MADT_header_t
{
    uint8_t type;
    uint8_t length;
} __attribute__ ((packed));

struct ACPI::MADT_lapic_t
{
    MADT_header_t header;
    uint8_t  processor_id;
    uint8_t  apic_id;
    uint64_t flags;
} __attribute__ ((packed));

struct ACPI::MADT_ioapic_t
{
    MADT_header_t header;
    uint8_t  ioapic_id;
    uint8_t  reserved;
    uint32_t ioapic_addr;
    uint32_t gsi_base;
} __attribute__ ((packed));

struct ACPI::MADT_ioapic_iso_t
{
    MADT_header_t header;
    uint8_t  bus_source;
    uint8_t  irq_source;
    uint32_t gsi;
    uint16_t flags;
}__attribute__((packed));



namespace ACPI
{
    struct Response
    {
        uint64_t lapic_base;
        uint64_t ioapic_base;
        uint64_t gsi_base;
    
        struct
        {
            size_t            lapic_count;
            size_t            ioapic_count;
            size_t            ioapic_iso_count;
            MADT_lapic_t      lapic[16];
            MADT_ioapic_t     ioapic[16];
            MADT_ioapic_iso_t ioapic_iso[16];
        } madt;

    };

    void init( uintptr_t acpi_phys, ACPI::Response& );

    struct GenericAddressStructure;
    struct FADT_t;

    namespace detail
    {
        template <typename rsdt_type>
        uintptr_t findTable( rsdt_type *rsdt, const char *signature );    
    }

    const auto findTableRSDT = detail::findTable<RSDT_t>;
    const auto findTableXSDT = detail::findTable<XSDT_t>;
}


























struct ACPI::GenericAddressStructure
{
    uint8_t AddressSpace;
    uint8_t BitWidth;
    uint8_t BitOffset;
    uint8_t AccessSize;
    uint64_t Address;
}  __attribute__ ((packed));


// Fixed ACPI Description Table
// https://wiki.osdev.org/FADT
struct ACPI::FADT_t
{
    ACPISDTHeader header;
    uint32_t FirmwareCtrl;
    uint32_t Dsdt;

    // field used in ACPI 1.0; no longer in use, for compatibility only
    uint8_t  Reserved;

    uint8_t  PreferredPowerManagementProfile;
    uint16_t SCI_Interrupt;
    uint32_t SMI_CommandPort;
    uint8_t  AcpiEnable;
    uint8_t  AcpiDisable;
    uint8_t  S4BIOS_REQ;
    uint8_t  PSTATE_Control;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t  PM1EventLength;
    uint8_t  PM1ControlLength;
    uint8_t  PM2ControlLength;
    uint8_t  PMTimerLength;
    uint8_t  GPE0Length;
    uint8_t  GPE1Length;
    uint8_t  GPE1Base;
    uint8_t  CStateControl;
    uint16_t WorstC2Latency;
    uint16_t WorstC3Latency;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t  DutyOffset;
    uint8_t  DutyWidth;
    uint8_t  DayAlarm;
    uint8_t  MonthAlarm;
    uint8_t  Century;

    // reserved in ACPI 1.0; used since ACPI 2.0+
    uint16_t BootArchitectureFlags;

    uint8_t  Reserved2;
    uint32_t Flags;

    // 12 byte structure; see below for details
    GenericAddressStructure ResetReg;

    uint8_t  ResetValue;
    uint8_t  Reserved3[3];
  
    // 64bit pointers - Available on ACPI 2.0+
    uint64_t                X_FirmwareControl;
    uint64_t                X_Dsdt;

    GenericAddressStructure X_PM1aEventBlock;
    GenericAddressStructure X_PM1bEventBlock;
    GenericAddressStructure X_PM1aControlBlock;
    GenericAddressStructure X_PM1bControlBlock;
    GenericAddressStructure X_PM2ControlBlock;
    GenericAddressStructure X_PMTimerBlock;
    GenericAddressStructure X_GPE0Block;
    GenericAddressStructure X_GPE1Block;
}  __attribute__ ((packed));















#include <type_traits>

template <typename rsdt_type>
uintptr_t ACPI::detail::findTable( rsdt_type *rsdt, const char *signature )
{
    static_assert(
        std::is_same_v<rsdt_type, ACPI::RSDT_t>
        || std::is_same_v<rsdt_type, ACPI::XSDT_t>
    );

    constexpr size_t N = std::is_same_v<rsdt_type, ACPI::RSDT_t> ? 4 : 8;

    syslog log("ACPI::findTable");
    int entries = (rsdt->header.Length - sizeof(rsdt->header)) / N;
    // log("entries: %d", entries);

    for (int i=0; i<entries; i++)
    {
        uintptr_t phys = (uintptr_t)(rsdt->PointerToOtherSDT[i]);
        uintptr_t page = idk::align_down(phys, PMM::PAGE_SIZE);
        VMM::mapPage(page, page);

        auto *header = (ACPISDTHeader*)phys;
        char buf[5];
        memcpy(buf, header->Signature, 4);
        buf[4] = '\0';
        log("signature: \"%s\"", buf);
        log("phys: 0x%lx", phys);
        log("page: 0x%lx", page);

        if (strncmp(header->Signature, signature, 4) == 0)
        {
            return phys;
        }
    }
    return 0;
}



