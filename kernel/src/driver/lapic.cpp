// ------------------------------------------------------------------------------------------------
// intr/local_apic.c
// ------------------------------------------------------------------------------------------------

#include <driver/lapic.hpp>
#include <kernel/ioport.hpp>
#include <kernel/memory/pmm.hpp>

// ------------------------------------------------------------------------------------------------
// Local APIC Registers
#define LAPIC_ID                        0x0020  // Local APIC ID
#define LAPIC_VER                       0x0030  // Local APIC Version
#define LAPIC_TPR                       0x0080  // Task Priority
#define LAPIC_APR                       0x0090  // Arbitration Priority
#define LAPIC_PPR                       0x00a0  // Processor Priority
#define LAPIC_EOI                       0x00b0  // EOI
#define LAPIC_RRD                       0x00c0  // Remote Read
#define LAPIC_LDR                       0x00d0  // Logical Destination
#define LAPIC_DFR                       0x00e0  // Destination Format
#define LAPIC_SVR                       0x00f0  // Spurious Interrupt Vector
#define LAPIC_ISR                       0x0100  // In-Service (8 registers)
#define LAPIC_TMR                       0x0180  // Trigger Mode (8 registers)
#define LAPIC_IRR                       0x0200  // Interrupt Request (8 registers)
#define LAPIC_ESR                       0x0280  // Error Status
#define LAPIC_ICRLO                     0x0300  // Interrupt Command
#define LAPIC_ICRHI                     0x0310  // Interrupt Command [63:32]
#define LAPIC_TIMER                     0x0320  // LVT Timer
#define LAPIC_THERMAL                   0x0330  // LVT Thermal Sensor
#define LAPIC_PERF                      0x0340  // LVT Performance Counter
#define LAPIC_LINT0                     0x0350  // LVT LINT0
#define LAPIC_LINT1                     0x0360  // LVT LINT1
#define LAPIC_ERROR                     0x0370  // LVT Error
#define LAPIC_TICR                      0x0380  // Initial Count (for Timer)
#define LAPIC_TCCR                      0x0390  // Current Count (for Timer)
#define LAPIC_TDCR                      0x03e0  // Divide Configuration (for Timer)

// ------------------------------------------------------------------------------------------------
// Interrupt Command Register

// Delivery Mode
#define ICR_FIXED                       0x00000000
#define ICR_LOWEST                      0x00000100
#define ICR_SMI                         0x00000200
#define ICR_NMI                         0x00000400
#define ICR_INIT                        0x00000500
#define ICR_STARTUP                     0x00000600

// Destination Mode
#define ICR_PHYSICAL                    0x00000000
#define ICR_LOGICAL                     0x00000800

// Delivery Status
#define ICR_IDLE                        0x00000000
#define ICR_SEND_PENDING                0x00001000

// Level
#define ICR_DEASSERT                    0x00000000
#define ICR_ASSERT                      0x00004000

// Trigger Mode
#define ICR_EDGE                        0x00000000
#define ICR_LEVEL                       0x00008000

// Destination Shorthand
#define ICR_NO_SHORTHAND                0x00000000
#define ICR_SELF                        0x00040000
#define ICR_ALL_INCLUDING_SELF          0x00080000
#define ICR_ALL_EXCLUDING_SELF          0x000c0000

// Destination Field
#define ICR_DESTINATION_SHIFT           24




uint32_t lapic_read_register(uint32_t reg)
{
    uintptr_t lapic_base = (uintptr_t)0xfee00000 + PMM::hhdm;
    return *((volatile uint32_t *)lapic_base + reg);
}

// write to a local apic manager
// currently not laid out for x2apic, otherwise would check
// if supported and then use msr
void lapic_write_register(uint32_t reg, uint32_t data)
{
    uintptr_t lapic_base = (uintptr_t)0xfee00000 + PMM::hhdm;
    *((volatile uint32_t *)(lapic_base + reg)) = data;
}

// ------------------------------------------------------------------------------------------------
void LAPIC::enable()
{
    // Clear task priority to enable all interrupts
    lapic_write_register(LAPIC_TPR, 0);

    // Logical Destination Mode
    lapic_write_register(LAPIC_DFR, 0xffffffff);   // Flat mode
    lapic_write_register(LAPIC_LDR, 0x01000000);   // All cpus use logical id 1

    // Configure Spurious Interrupt Vector Register
    lapic_write_register(LAPIC_SVR, 0x100 | 0xff);
}

// ------------------------------------------------------------------------------------------------
uint32_t LocalApicGetId()
{
    return lapic_read_register(LAPIC_ID) >> 24;
}

// ------------------------------------------------------------------------------------------------
void LocalApicSendInit( uint32_t apic_id )
{
    lapic_write_register(LAPIC_ICRHI, apic_id << ICR_DESTINATION_SHIFT);
    lapic_write_register(LAPIC_ICRLO, ICR_INIT | ICR_PHYSICAL
        | ICR_ASSERT | ICR_EDGE | ICR_NO_SHORTHAND);

    while (lapic_read_register(LAPIC_ICRLO) & ICR_SEND_PENDING)
    {

    }
}

// ------------------------------------------------------------------------------------------------
void LocalApicSendStartup( uint32_t apic_id, uint32_t vector )
{
    lapic_write_register(LAPIC_ICRHI, apic_id << ICR_DESTINATION_SHIFT);
    lapic_write_register(LAPIC_ICRLO, vector | ICR_STARTUP
        | ICR_PHYSICAL | ICR_ASSERT | ICR_EDGE | ICR_NO_SHORTHAND);

    while (lapic_read_register(LAPIC_ICRLO) & ICR_SEND_PENDING)
    {

    }
}




    
// uint32_t IOAPIC::read(void *ioapicaddr, uint32_t reg)
// {
//     uint32_t volatile *ioapic = (uint32_t volatile *)ioapicaddr;
//     ioapic[0] = (reg & 0xff);
//     return ioapic[4];
// }

// void IOAPIC::write(void *ioapicaddr, uint32_t reg, uint32_t value)
// {
//     uint32_t volatile *ioapic = (uint32_t volatile *)ioapicaddr;
//     ioapic[0] = (reg & 0xff);
//     ioapic[4] = value;
// }

