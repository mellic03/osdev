// // ------------------------------------------------------------------------------------------------
// // intr/ioapic.c
// // ------------------------------------------------------------------------------------------------
// #include <stdint.h>
#include <driver/ioapic.hpp>
#include <kernel/ioport.hpp>
#include <kernel/log.hpp>
#include <string.h>

// ------------------------------------------------------------------------------------------------
// Globals
uint8_t *g_ioApicAddr;

// ------------------------------------------------------------------------------------------------
// Memory mapped registers for IO APIC register access
#define IOREGSEL                        0x00
#define IOWIN                           0x10

// ------------------------------------------------------------------------------------------------
// IO APIC Registers
#define IOAPICID                        0x00
#define IOAPICVER                       0x01
#define IOAPICARB                       0x02
#define IOREDTBL                        0x10

// ------------------------------------------------------------------------------------------------
static void IoApicOut(uint8_t *base, uint8_t reg, uint32_t val)
{
    MMIO::out32(base + IOREGSEL, reg);
    MMIO::out32(base + IOWIN, val);
}

// ------------------------------------------------------------------------------------------------
static uint32_t IoApicIn(uint8_t *base, uint8_t reg)
{
    MMIO::out32(base + IOREGSEL, reg);
    return MMIO::in32(base + IOWIN);
}

// ------------------------------------------------------------------------------------------------
void IOAPIC::setEntry(uint8_t *base, uint8_t index, uint64_t data)
{
    IoApicOut(base, IOREDTBL + index * 2, (uint32_t)data);
    IoApicOut(base, IOREDTBL + index * 2 + 1, (uint32_t)(data >> 32));
}




uint64_t detect_ioapic( uint8_t *rsdt )
{
    syslog log("detect_ioapic");
    if (rsdt == NULL) {
        log("RSDT not found\n");
    }
    uint8_t *ptr, *ptr2;
    uint32_t len;
    for(len = *((uint32_t*)(rsdt + 4)), ptr2 = rsdt + 36; ptr2 < rsdt + len; ptr2 += rsdt[0]=='X' ? 8 : 4) {
        ptr = (uint8_t*)(uintptr_t)(rsdt[0]=='X' ? *((uint64_t*)ptr2) : *((uint32_t*)ptr2));
        if(!memcmp(ptr, "APIC", 4)) {
            // uint32_t *ptr32 = (uint32_t*)((uint8_t*)ptr + 0x24);
            // uint32_t val32 = *ptr32;
            // uint64_t lapic_ptr = (uint64_t)val32;
            ptr2 = ptr + *((uint32_t*)(ptr + 4));
            for(ptr += 44; ptr < ptr2; ptr += ptr[1]) {
                if (ptr[0] == 1) {
                    uint64_t ioapic_ptr = (uint64_t)*((uint32_t*)(ptr+4));
                    return ioapic_ptr;
                }
            }
            break;
        } else {
            log("Could not locate IOAPIC\n");
            return 0;
        }
    }
    return 0;
}





// // ------------------------------------------------------------------------------------------------
void IOAPIC::enable( uint8_t *rsdtptr )
{
    syslog log("IOAPIC::enable");
    uint8_t *rsdt = rsdtptr;
    uint8_t *addr = (uint8_t*)detect_ioapic(rsdt);

    log("addr: 0x%lx", addr);
    g_ioApicAddr = addr;

    // Get number of entries supported by the IO APIC
    uint32_t x = IoApicIn(g_ioApicAddr, IOAPICVER);
    uint32_t count = ((x >> 16) & 0xff) + 1;    // maximum redirection entry

    log("I/O APIC pins = %d", count);

    // Disable all entries
    for (uint32_t i = 0; i < count; ++i)
    {
        setEntry(g_ioApicAddr, i, 1 << 16);
    }
}





