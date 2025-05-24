#pragma once
#include <stddef.h>
#include <stdint.h>

// https://wiki.osdev.org/APIC#IO_APIC_Registers

namespace ACPI
{
    struct Response;
}


namespace APIC
{
    static constexpr size_t REG_EOI      = 0xb0;
    static constexpr size_t REG_SPURIOUS = 0xf0;
    static constexpr size_t ENABLE       = (1 << 8);
    static constexpr size_t SPURIOUS_INT = 0xff;

    void init( const ACPI::Response& );
}


namespace LAPIC
{
    static constexpr size_t REG_EOI      = 0x0B0; // Signal EOI by writing 0 
    static constexpr size_t REG_SPURIOUS = 0x0F0;
    static constexpr size_t REG_ICR      = 0x300; // Interrupt Command Register
    static constexpr size_t REG_ICR0     = 0x300;
    static constexpr size_t REG_ICR1     = 0x310;

    void init( const ACPI::Response& );
}



namespace IOAPIC
{
    void     init( const ACPI::Response& );
    void     maskIRQ( uint8_t irqno );
    void     unmaskIRQ( uint8_t irqno );
    uint32_t read( uint32_t reg );
    void     write( uint32_t reg, uint32_t value );


    struct redirection_entry_regs
    {
        uint32_t lower;
        uint32_t upper;
    }__attribute__((packed));

    struct redirection_entry
    {
        uint64_t vector        : 8;
        uint64_t delivery_m    : 3;
        uint64_t destination_m : 1;
        uint64_t status_m      : 1;
        uint64_t polarity      : 1;
        uint64_t irr           : 1;
        uint64_t trigger       : 1;
        uint64_t mask          : 1;
        uint64_t reserved      : 39;
        uint64_t destination   : 8;
    }__attribute__((packed));

    union redirection_entry_t
    {
        struct redirection_entry entry;
        struct redirection_entry_regs registers;
        volatile uint64_t raw_data;
    }__attribute__((packed));

}

