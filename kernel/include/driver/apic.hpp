#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>

#define APIC_REG_ID             0x20
#define APIC_REG_VERSION        0x30
#define APIC_REG_SPURIOUS_INT   0xF0
#define APIC_REG_EOI            0xB0
#define APIC_REG_ICR_LO        0x300
#define APIC_REG_ICR_HIGH       0x310

#define APIC_SPURIOUS_VECTOR_NUM 0xFF
#define APIC_FLAG_ENABLE        (1 << 8)

#define APIC_IPI_TYPE_INIT      0b101
#define APIC_IPI_TYPE_STARTUP   0b110

extern volatile void *lapic_base;

void apic_init(void);
void apic_enable(void);
uint32_t apic_read_reg(uint16_t offset);
void apic_write_reg(uint16_t offset, uint32_t val);
void apic_send_eoi(void);
void apic_send_ipi(uint8_t dest, uint8_t vector, uint32_t mtype);


