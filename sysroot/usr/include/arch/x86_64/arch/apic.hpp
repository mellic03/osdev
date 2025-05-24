#pragma once
#include <stddef.h>
#include <stdint.h>

// https://wiki.osdev.org/APIC#IO_APIC_Registers


namespace APIC
{
    void init( uint64_t ioapic_base, uint64_t lapic_base );
}


namespace IOAPIC
{
    void     init( uint32_t ioapic_base );
    uint32_t read( uint32_t reg );
    void     write( uint32_t reg, uint32_t value );
}




namespace LAPIC
{
    // static constexpr size_t REG_EOI         = 0x0B0; // Signal EOI by writing 0 
    // static constexpr size_t REG_SPURIOUS    = 0x0F0;

    // static constexpr size_t REG_ICR         = 0x300; // Interrupt Command Register
    // static constexpr size_t REG_ICR0        = 0x300;
    // static constexpr size_t REG_ICR1        = 0x310;

    void init();
}