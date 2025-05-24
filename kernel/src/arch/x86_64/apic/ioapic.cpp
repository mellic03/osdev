#include <arch/apic.hpp>
#include <arch/acpi.hpp>
#include <driver/pic.hpp>
#include <kernel/memory/vmm.hpp>
#include <kernel/interrupt.hpp>


#define IOAPICID        0x00
#define IOAPICVER       0x01
#define IOAPICARB       0x02
#define IOAPICREDTBL(n) (0x10 + 2 * n) // lower-32bits (add +1 for upper 32-bits)

void ioapic_write_reg( const uint8_t offset, const uint32_t val );
uint32_t ioapic_read_reg( const uint8_t offset );



static void Write64( uint32_t reg, uint64_t data )
{
    uint32_t low = data & 0xFFFFFFFF;
    uint32_t high = data >> 32;

    ioapic_write_reg(reg, low);
    ioapic_write_reg(reg + 1, high);
}



void ioapic_writeRedirEntry( uint8_t irqno, IOAPIC::RedirEntry entry )
{
    union {
        struct {
            uint32_t lo;
            uint32_t hi;
        } split;

        IOAPIC::RedirEntry rdentry;
    } woopee;

    woopee.rdentry = entry;

    ioapic_write_reg(0x10 + 2*irqno+0, woopee.split.lo);
    ioapic_write_reg(0x10 + 2*irqno+1, woopee.split.hi);
}


void IOAPIC::init()
{
    syslog log("IOAPIC::init");

    // size_t count = res.madt.ioapic_count;
    // auto  &table = res.madt.ioapic;

    // for (int i=0; i<count; i++)
    // {
    //     table[i].
    // }

}

void IOAPIC::mapIRQ( uint8_t irqno )
{
    uint32_t delivery = 0;
    uint8_t  isrno    = IntNo_IOAPIC_Base + irqno;
    Write64(IOAPICREDTBL(irqno), delivery|isrno);
}




// https://wiki.osdev.org/IOAPIC#Programming_the_I/O_APIC


void ioapic_write_reg( const uint8_t offset, const uint32_t val )
{
    *(volatile uint32_t*)(APIC::ioapicBase) = offset;
    *(volatile uint32_t*)(APIC::ioapicBase + 0x10) = val; 
}
 
uint32_t ioapic_read_reg( const uint8_t offset )
{
    *(volatile uint32_t*)(APIC::ioapicBase) = offset;
    return *(volatile uint32_t*)(APIC::ioapicBase + 0x10);
}
