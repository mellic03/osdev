#pragma once
#include <kdef.h>

extern uint8_t *g_ioApicAddr;

namespace IOAPIC
{
    uint32_t read(void *ioapicaddr, uint32_t reg);
    void     write(void *ioapicaddr, uint32_t reg, uint32_t value);

    void enable(  uint8_t *rsdtptr );
    void setEntry(uint8_t *base, uint8_t index, uint64_t data);
}