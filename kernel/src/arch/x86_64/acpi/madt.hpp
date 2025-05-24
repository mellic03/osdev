#pragma once
#include "acpi.hpp"


namespace ACPI
{
    struct MADT_t;
    void MADT_init( MADT_t*, ACPI::Response& );

    struct MADT_header_t;
    struct MADT_lapic_t;
    struct MADT_ioapic_t;
}

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



// /* MADT Record Header */
// typedef struct[[gnu::packed]] {
//     uint8_t type;
//     uint8_t len;
// } madt_record_hdr_t;


// /* I/O APIC */
// typedef struct[[gnu::packed]] {
//     madt_record_hdr_t hdr;

//     uint8_t id;
//     uint8_t reserved;
//     uint32_t addr;
//     uint32_t gsi_base;
// } madt_record_ioapic_t;

// /* Interrupt Source Override */
// typedef struct[[gnu::packed]] {
//     madt_record_hdr_t hdr;

//     uint8_t bus_src;
//     uint8_t irq_src;
//     uint32_t gsi;
//     uint16_t flags;
// } madt_record_iso_t;

// /* Non Maskable Interrupt */
// typedef struct[[gnu::packed]] {
//     madt_record_hdr_t hdr;

//     uint8_t proc_id;
//     uint16_t flags;
//     uint8_t lint;
// } madt_record_nmi_t;
