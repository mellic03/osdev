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
    // static constexpr size_t REG_EOI         = 0x0B0; // Signal EOI by writing 0 
    // static constexpr size_t REG_SPURIOUS    = 0x0F0;
    // static constexpr size_t REG_ICR         = 0x300; // Interrupt Command Register
    // static constexpr size_t REG_ICR_LO      = 0x300; // Low bits
    // static constexpr size_t REG_ICR_HI      = 0x310; // High bits
    // static constexpr size_t REG_LVT_TIMER   = 0x320;
    // static constexpr size_t REG_LVT_THERMAL = 0x330;
    // static constexpr size_t REG_LVT_PERF    = 0x340;
    // static constexpr size_t REG_LVT_LINT0   = 0x350;
    // static constexpr size_t REG_LVT_LINT1   = 0x360;
    // static constexpr size_t REG_LVT_ERROR   = 0x360;

    // static constexpr size_t REG_TIMER_INIT  = 0x380; // Initial count register (timer)
    // static constexpr size_t REG_TIMER_CURR  = 0x390; // Current count register (timer)
    // static constexpr size_t REG_TIMER_DIV   = 0x3E0; // Divide config register (timer)


    static constexpr uint32_t REG_APIC_ID     =  0x020; // APIC ID Register
    static constexpr uint32_t REG_APIC_VER    =  0x030; // APIC Version Register
    static constexpr uint32_t REG_TPR         =  0x080; // Task Priority Register 
    static constexpr uint32_t REG_APR         =  0x090; // Arbitration Priority Register
    static constexpr uint32_t REG_PPR         =  0x0A0; // Processor Priority Register
    static constexpr uint32_t REG_EOI         =  0x0B0; // End of Interrupt Register
    static constexpr uint32_t REG_RRD         =  0x0C0; // Remote Read Register
    static constexpr uint32_t REG_LDR         =  0x0D0; // Logical Destination Register
    static constexpr uint32_t REG_DFR         =  0x0E0; // Destination Format Register
    static constexpr uint32_t REG_SPURIOUS    =  0x0F0; // Spurious Interrupt Vector Register
    static constexpr uint32_t REG_ISR         =  0x100; // In-service Register
    static constexpr uint32_t REG_TMR         =  0x180; // Trigger Mode Register
    static constexpr uint32_t REG_IRR         =  0x200; // Interrupt Request Register
    static constexpr uint32_t REG_ERRSTAT     =  0x280; // Error Status Register
    static constexpr uint32_t REG_ICR_LO      =  0x300; // Interrupt Command Register Low
    static constexpr uint32_t REG_ICR_HI      =  0x310; // Interrupt Command Register High
    static constexpr uint32_t REG_LVT_TIMER   =  0x320; // Timer Local Vector Table Entry
    static constexpr uint32_t REG_LVT_THERMAL =  0x330; // Thermal Local Vector Table Entry
    static constexpr uint32_t REG_LVT_PERFMON =  0x340; // Performance monitoring Local Vector Table Entry
    static constexpr uint32_t REG_LVT_LINT0   =  0x350; // Local Interrupt 0 Local Vector Table Entry
    static constexpr uint32_t REG_LVT_LINT1   =  0x360; // Local Interrupt 1 Local Vector Table Entry
    static constexpr uint32_t REG_LVT_ERROR   =  0x370; // Error Local Vector Table Entry
    static constexpr uint32_t REG_TIMER_INIT  =  0x380; // Timer Initial Count Register
    static constexpr uint32_t REG_TIMER_CURR  =  0x390; // Timer Current Count Register
    static constexpr uint32_t REG_TIMER_DIV   =  0x3E0; // Timer Divide Configuration Register

    static constexpr uint32_t TIMER_ONESHOT  = 00000;
    static constexpr uint32_t TIMER_PERIODIC = (1<<17);
    
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


    #define ICR_VECTOR(x) (x & 0xFF)
    #define ICR_MESSAGE_TYPE_FIXED 0
    #define ICR_MESSAGE_TYPE_LOW_PRIORITY (1 << 8)
    #define ICR_MESSAGE_TYPE_SMI (2 << 8)
    #define ICR_MESSAGE_TYPE_REMOTE_READ (3 << 8)
    #define ICR_MESSAGE_TYPE_NMI (4 << 8)
    #define ICR_MESSAGE_TYPE_INIT (5 << 8)
    #define ICR_MESSAGE_TYPE_STARTUP (6 << 8)
    #define ICR_MESSAGE_TYPE_EXTERNAL (7 << 8)

    #define ICR_DSH_DEST 0          // Use destination field
    #define ICR_DSH_SELF (1 << 18)  // Send to self
    #define ICR_DSH_ALL (2 << 18)   // Send to ALL APICs
    #define ICR_DSH_OTHER (3 << 18) // Send to all OTHER APICs 

    // LAPIC::SendIPI(0, ICR_DSH_OTHER, ICR_MESSAGE_TYPE_FIXED, isrno);
    // SendIPI(lapic_id, ICR_DSH_DEST, ICR_MESSAGE_TYPE_FIXED, isrno);

    void init();
    void startTimer();
    void resetTimer( uint32_t us );

    void sendIPI( uint32_t lapic_id, uint8_t isrno, uint32_t dsh=ICR_DSH_DEST,
                  uint32_t type=ICR_MESSAGE_TYPE_FIXED );
    void sendEOI();
}









// #define IO_APIC_REGSEL 0x00 // I/O APIC Register Select Address Offset
// #define IO_APIC_WIN 0x10 // I/O APIC I/O Window Address offset

// #define IO_APIC_REGISTER_ID 0x0 // ID Register
// #define IO_APIC_REGISTER_VER 0x1 // Version Register
// #define IO_APIC_REGISTER_ARB 0x2 // I/O APIC Arbitration ID
// #define IO_APIC_RED_TABLE_START 0x10 // I/O APIC Redirection Table Start
// #define IO_APIC_RED_TABLE_ENT(x) (0x10 + 2 * x)

// #define IO_RED_TBL_VECTOR(x) (x & 0xFF)







namespace IOAPIC
{
    static constexpr uint32_t REG_SEL       = 0x0;
    static constexpr uint32_t REG_IOAPICVER = 0x1;

    void     init();
    void     mapIRQ( uint8_t lapic_id, uint8_t irqno, bool mask );
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

