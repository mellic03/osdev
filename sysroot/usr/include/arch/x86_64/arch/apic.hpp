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
    extern uintptr_t lapicBase;
    extern uintptr_t ioapicBase;
    extern uintptr_t gsiBase;

    static constexpr size_t ENABLE       = (1 << 8);
    static constexpr size_t SPURIOUS_INT = 0xff;

    // https://github.com/dreamportdev/Osdev-Notes/blob/master/02_Architecture/07_APIC.md
    static constexpr size_t REG_EOI         = 0x0B0; // Signal EOI by writing 0 
    static constexpr size_t REG_SPURIOUS    = 0x0F0;
    static constexpr size_t REG_ICR         = 0x300; // Interrupt Command Register
    static constexpr size_t REG_ICR_LO      = 0x300; // Low bits
    static constexpr size_t REG_ICR_HI      = 0x310; // High bits

    /*
        Timer: used for controlling the local APIC timer. Offset: 0x320.
        Thermal Monitor: used for configuring interrupts when certain thermal
            conditions are met. Offset: 0x330.
        Performance Counter: allows an interrupt to be generated when
            a performance counter overflows. Offset: 0x340.
        LINT0: Specifies the interrupt delivery when an interrupt
            is signaled on LINT0 pin.Offset: 0x350.
        LINT1: Specifies the interrupt delivery when an interrupt
            is signaled on LINT1 pin. Offset: 0x360.
        Error: configures how the local APIC should report an
            internal error, Offset 0x370.
    */
    static constexpr size_t REG_LVT_TIMER   = 0x320;
    static constexpr size_t REG_LVT_THERMAL = 0x330;
    static constexpr size_t REG_LVT_PERF    = 0x340;
    static constexpr size_t REG_LVT_LINT0   = 0x350;
    static constexpr size_t REG_LVT_LINT1   = 0x360;
    static constexpr size_t REG_LVT_ERROR   = 0x360;

    static constexpr size_t REG_TIMER_INIT  = 0x380; // Initial count register (timer)
    static constexpr size_t REG_TIMER_CURR  = 0x390; // Current count register (timer)
    static constexpr size_t REG_TIMER_DIV   = 0x3E0; // Divide config register (timer)

    static constexpr size_t TMR_ONESHOT  = 00000;
    static constexpr size_t TMR_PERIODIC = (1<<17);
    
    void init( const ACPI::Response& );
    void write( uint32_t reg, uint32_t data );
    uint32_t read( uint32_t reg );
    
}









namespace LAPIC
{
    /*
        Interrupt Vector:   This is the IDT entry we want to trigger when for this interrupt.
        Delivery mode:      determines how the APIC should present the interrupt to the processor.
        Destination Mode:   Can be either physical or logical.
        Delivery Status:    whether the interrupt has been served or not (Read Only).
        Pin polarity:       0 is active-high, 1 is active-low.
        Remote IRR:         used by the APIC for managing level-triggered interrupts (Read Only).
        Trigger mode:       0 is edge-triggered, 1 is level-triggered.
        Interrupt mask:     if it is 1 the interrupt is disabled, if 0 is enabled.
    */

    // struct lvt_entry_t
    // {
    //     uint8_t isrno            : 8;
    //     uint8_t delivery_mode    : 2;
    //     uint8_t destination_mode : 1;
    //     uint8_t delivery_status  : 1;
    //     uint8_t pin_polarity     : 1;
    //     uint8_t remote_irr       : 1;
    //     uint8_t trigger_mode     : 1;
    //     uint8_t interrupt_mask   : 1;
    // };

    void init( uint32_t usecs );
    void resetTimer( uint32_t us );
    void sendIPI( uint32_t lapic_id, uint8_t isrno );
    void sendEOI();
}






namespace IOAPIC
{
    static constexpr uint32_t REG_SEL       = 0x0;
    static constexpr uint32_t REG_IOAPICVER = 0x1;

    void     init();
    void     mapIRQ( uint8_t lapic_id, uint8_t irqno );
    void     maskIRQ( uint8_t irqno );
    void     unmaskIRQ( uint8_t irqno );
    uint32_t read( uint32_t reg );
    void     write( uint32_t reg, uint32_t value );


    struct redirection_entry_regs
    {
        uint32_t lower;
        uint32_t upper;
    }__attribute__((packed));

    struct RedirEntry
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
        struct RedirEntry entry;
        struct redirection_entry_regs registers;
        volatile uint64_t raw_data;
    }__attribute__((packed));

}

