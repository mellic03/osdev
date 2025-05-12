#include "madt.hpp"
#include <kernel/log.hpp>
#include <kmalloc.h>
#include <string.h>


const char *MADT_type_str( uint8_t type )
{
    switch (type)
    {
        default: return "INVALID";
        case 0: return "Processor Local APIC";
        case 1: return "I/O APIC";
        case 2: return "I/O APIC Interrupt Source Override";
        case 3: return "I/O APIC Non-maskable interrupt source";
        case 4: return "Local APIC Non-maskable interrupts";
        case 5: return "Local APIC Address Override";
        case 9: return "Processor Local x2APIC";
    }
}



void LAPIC_init( ACPI::MADT_lapic_t *info )
{
    syslog log("LAPIC_init");
    log("processor_id: %u", info->processor_id);
    log("apic_id:      %u", info->apic_id);
    log("flags:        %lu", info->flags);
}



void IOAPIC_init( ACPI::MADT_lapic_t *info )
{
    syslog log("LAPIC_init");
    log("processor_id: %u", info->processor_id);
    log("apic_id:      %u", info->apic_id);
    log("flags:        %lu", info->flags);
}





void ACPI::MADT_init( ACPI::MADT_t *madt )
{
    syslog log("MADT_init");

    // If bit 0 in the flags field is set then you need to mask all the 8259 PIC's interrupts,
    // but you should probably do this anyway. 
    if (madt->flags & (1<<0))
    {
        // PIC::disable();
    }

    log("lapic_addr: 0x%lx", madt->lapic_addr);
    log("flags:      %lu", madt->flags);


    uintptr_t base = (uintptr_t)(madt->records);
    uintptr_t end  = base + madt->header.Length;
    uintptr_t curr = base + 0;

    log("end: 0x%lx", end);

    while (curr < end)
    {
        auto *entry = (MADT_header_t*)curr;

        log("entry: 0x%lx, type %u: %s", curr, entry->type, MADT_type_str(entry->type));
    
        if (entry->type == 0)
        {
            LAPIC_init((MADT_lapic_t*)entry);
        }

        curr += entry->length;
    }

}





// template <typename source_table, typename curr_table>
// void *nextTable( source_table *parent, curr_table *curr )
// {
//     syslog log("ACPI::findTable");
//     int entries = (rsdt->header.Length - sizeof(rsdt->header)) / 4;
//     log("entries: %d", entries);

//     for (int i=0; i<entries; i++)
//     {
//         uintptr_t phys = (uintptr_t)(rsdt->PointerToOtherSDT[i]);
//         uintptr_t virt = (uintptr_t)phys + PMM::hhdm;
//         log("phys: 0x%lx", phys);
//         log("virt: 0x%lx", virt);
//         auto *header = (ACPISDTHeader*)virt;

//         char buf[5];
//         memcpy(buf, header->Signature, 4);
//         buf[4] = '\0';
//         log("signature: \"%s\"", buf);

//         if (strncmp(header->Signature, signature, 4) == 0)
//             return (table_type*)header;
//     }
//     return nullptr;
// }















