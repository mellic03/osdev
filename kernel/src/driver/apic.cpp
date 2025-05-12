#include <driver/acpi.hpp>
#include <driver/apic.hpp>
#include <driver/madt.hpp>
#include <driver/pic.hpp>
#include <cpu/cpu.hpp>
#include <cpu/idt.hpp>
#include <kernel/ioport.hpp>
#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>
#include <kernel/log.hpp>
#include <kernel/bitmanip.hpp>
#include <kpanic.h>
#include <string.h>

#define IA32_APIC_BASE_MSR          0x1B
#define IA32_APIC_BASE_MSR_BSP      0x100       /* Processor is a BSP */
#define IA32_APIC_BASE_MSR_X2APIC   0x400
#define IA32_APIC_BASE_MSR_ENABLE   0x800

// static bool is_x2apic_enabled = false;

/* The local APIC registers are memory mapped to an address that can be found
 * in the MP/MADT tables.
 */
volatile void *lapic_base = NULL;

/**
 * Reads the value of a register from the Advanced Programmable Interrupt
 * Controller (APIC).
 *
 * @param offset The offset of the register to read.
 *
 * @returns The value of the register.
 */
uint32_t apic_read_reg(uint16_t offset)
{
    return *((uint32_t volatile *)(lapic_base) + offset);
}

/**
 * Writes a value to a register in the Advanced Programmable Interrupt
 * Controller (APIC).
 *
 * @param offset The offset of the register.
 * @param val The value to write to the register.
 *
 * @returns None
 */
void apic_write_reg(uint16_t offset, uint32_t val)
{
    *(((uint32_t volatile *)(lapic_base) + offset)) = val;
}

/**
 * Sends an End of Interrupt (EOI) signal to the Advanced Programmable Interrupt
 * Controller (APIC).
 *
 * @returns None
 */
void apic_send_eoi()
{
    apic_write_reg(APIC_REG_EOI, 1);
}

/**
 * Sends an Inter-Processor Interrupt (IPI) to a specific destination processor.
 *
 * @param dest The destination processor ID.
 * @param vector The interrupt vector number.
 * @param mtype The message type for the IPI.
 *
 * @returns None
 */
void apic_send_ipi(uint8_t dest, uint8_t vector, uint32_t mtype)
{
    apic_write_reg(APIC_REG_ICR_HIGH, (uint32_t) dest << 24);
    apic_write_reg(APIC_REG_ICR_LOW, (mtype << 8) | vector);
}


/**
 * Enables the Advanced Programmable Interrupt Controller (APIC).
 *
 * This function writes a value to the Spurious Interrupt Vector Register (SVR)
 * of the APIC, enabling it and setting the interrupt vector number to the
 * APIC_SPURIOUS_VECTOR_NUM constant.
 *
 * @returns None
 */
void apic_enable()
{
    apic_write_reg(APIC_REG_SPURIOUS_INT,
                   APIC_FLAG_ENABLE | APIC_SPURIOUS_VECTOR_NUM);
}














