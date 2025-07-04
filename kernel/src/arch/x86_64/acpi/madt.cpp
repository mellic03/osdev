#include <arch/acpi.hpp>
#include <kernel/log.hpp>
#include <string.h>


// static const char *MADT_type_str( uint8_t );
static void LAPIC_init( ACPI::MADT_lapic_t*, ACPI::Response& );
static void IOAPIC_init( ACPI::MADT_ioapic_t*, ACPI::Response& );
static void IOAPIC_ISO_init( ACPI::MADT_ioapic_iso_t*, ACPI::Response& );



void ACPI::MADT_init( ACPI::MADT_t *madt, ACPI::Response &res )
{
    syslog log("MADT_init");

    // If bit 0 in the flags field is set then you need to mask all the 8259 PIC's interrupts,
    // but you should probably do this anyway. 
    if (madt->flags & (1<<0))
    {
        // PIC::disable();
    }

    uintptr_t base = (uintptr_t)(madt->records);
    uintptr_t end  = base + madt->header.Length;
    uintptr_t curr = base + 0;

    log("lapic_addr: 0x%lx", madt->lapic_addr);
    // log("flags:      %lu", madt->flags);
    // log("end:        0x%lx", end);

    res.lapic_base = madt->lapic_addr;
    res.madt.lapic_count = 0;
    res.madt.ioapic_count = 0;
    res.madt.ioapic_iso_count = 0;

    // bool prev = syslog::isEnabled();
    // syslog::disable();

    while (curr < end)
    {
        auto *entry = (MADT_header_t*)curr;
        // if (0)
        // {
        //     log(
        //         "entry: 0x%lx, type %u: %s",
        //         curr, entry->type, MADT_type_str(entry->type)
        //     );
        // }

        switch (entry->type)
        {
            default: break;
            case 0: LAPIC_init((MADT_lapic_t*)entry, res);   break;
            case 1: IOAPIC_init((MADT_ioapic_t*)entry, res); break;
            case 2: IOAPIC_ISO_init((MADT_ioapic_iso_t*)entry, res); break;
        }

        curr += entry->length;
    }

    // if (prev)
    //     syslog::enable();
}




static void LAPIC_init( ACPI::MADT_lapic_t *info, ACPI::Response &res )
{
    // syslog log("MADT_LAPIC_init");
    // log("processor_id: %u",  info->processor_id);
    // log("apic_id:      %u",  info->apic_id);
    // log("flags:        %lu", info->flags);

    size_t &idx = res.madt.lapic_count;
    res.madt.lapic[idx++] = *info;
}



static void IOAPIC_init( ACPI::MADT_ioapic_t *info, ACPI::Response &res )
{
    // syslog log("MADT_IOAPIC_init");
    // log("ioapic_id:   %u", info->ioapic_id);
    // log("ioapic_addr: 0x%lx", info->ioapic_addr);
    // log("gsi_base:    %lu", info->gsi_base);

    res.ioapic_base = info->ioapic_addr;
    res.gsi_base    = info->gsi_base;

    size_t &idx = res.madt.ioapic_count;
    res.madt.ioapic[idx++] = *info;
}


static void IOAPIC_ISO_init( ACPI::MADT_ioapic_iso_t *info, ACPI::Response &res )
{
    // syslog log("MADT_IOAPIC_ISO_init");
    // log("bus_source: %u",    info->bus_source);
    // log("irq_source: 0x%lx", info->irq_source);
    // log("gsi:        %lu",   info->gsi);
    // log("flags:      %lu",   info->flags);

    size_t &idx = res.madt.ioapic_iso_count;
    res.madt.ioapic_iso[idx++] = *info;
}



// static const char *MADT_type_str( uint8_t type )
// {
//     switch (type)
//     {
//         default: return "INVALID";
//         case 0: return "Processor Local APIC";
//         case 1: return "I/O APIC";
//         case 2: return "I/O APIC Interrupt Source Override";
//         case 3: return "I/O APIC Non-maskable interrupt source";
//         case 4: return "Local APIC Non-maskable interrupts";
//         case 5: return "Local APIC Address Override";
//         case 9: return "Processor Local x2APIC";
//     }
// }


