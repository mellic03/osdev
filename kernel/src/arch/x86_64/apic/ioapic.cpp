#include <arch/apic.hpp>
#include <arch/acpi.hpp>
#include <driver/pic.hpp>
#include <kernel/memory/vmm.hpp>
#include <sys/interrupt.hpp>


#define IOAPICID        0x00
#define IOAPICVER       0x01
#define IOAPICARB       0x02
#define IOAPICREDTBL(n) (0x10 + 2*(uint32_t)n) // lower-32bits (add +1 for upper 32-bits)

void ioapic_write_reg( const uint8_t offset, const uint32_t val );
uint32_t ioapic_read_reg( const uint8_t offset );


static void Write64( uint32_t reg, uint64_t data )
{
    uint32_t low = data & 0xFFFFFFFF;
    uint32_t high = data >> 32;

    ioapic_write_reg(reg, low);
    ioapic_write_reg(reg + 1, high);
}


static uint64_t Read64( uint32_t reg )
{
    uint64_t lo = ioapic_read_reg(reg);
    uint64_t hi = ioapic_read_reg(reg+1);

    return (hi<<32) + lo;
}



void IOAPIC::init()
{
    // uint8_t redirEntry = (uint8_t)(ioapic_read_reg(IOAPICVER) >> 16) + 1;
    // syslog log("IOAPIC::init");
    // log("redirEntry: %u", redirEntry);

}

void IOAPIC::mapIRQ( uint8_t lapic_id, uint8_t irqno, bool mask )
{
    uint8_t isrno = IntNo_IrqBase + irqno;

    union {
        uint64_t   qword;
        RedirEntry entry;
    } U = {0};

    U.entry = {
        .vector        = isrno,
        .delivery_m    = 0,
        .destination_m = 0,
        .status_m      = 0,
        .polarity      = 0,
        .irr           = 0,
        .trigger       = 0,
        .mask          = mask,
        .reserved      = 0,
        .destination   = lapic_id,
    };

    Write64(IOAPICREDTBL(irqno), U.qword);
}

void IOAPIC::maskIRQ( uint8_t irqno )
{
    uint32_t reg = IOAPICREDTBL(irqno);

    union {
        uint64_t   qword;
        RedirEntry entry;
    } U = { Read64(reg) };
    U.entry.mask = 1;

    Write64(reg, U.qword);
}


void IOAPIC::unmaskIRQ( uint8_t irqno )
{
    uint32_t reg = IOAPICREDTBL(irqno);

    union {
        uint64_t   qword;
        RedirEntry entry;
    } U = { Read64(reg) };
    U.entry.mask = 0;

    Write64(reg, U.qword);
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
