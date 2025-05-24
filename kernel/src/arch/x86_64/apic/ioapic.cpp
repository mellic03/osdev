#include <arch/apic.hpp>
#include <arch/acpi.hpp>
#include <driver/pic.hpp>
#include <kernel/memory/vmm.hpp>


#define IO_WIN_ADDR 0xfec00010
#define IO_REG_SEL_ADDR 0xfec00000

#define NUM_OF_IRQ 16
#define IOAPICID 0x00
#define IOAPICVER 0x01
#define IOAPICARB 0x02
#define IOREDTBL_BASE 0x03
#define IDT_EXCEPTIONS 32

static volatile uintptr_t g_ioBase;
static volatile uintptr_t g_gsiBase;
static volatile uint8_t   g_maxRedirectionEntry;
static volatile uint32_t *g_ioRegSel; // = (uint32_t*)(IO_REG_SEL_ADDR + KERNEL_VIRT_BASE_ADDR);
static volatile uint32_t *g_ioWin; // = (uint32_t*)(IO_WIN_ADDR + KERNEL_VIRT_BASE_ADDR);
static ACPI::Response acpires;


static int irq_map[NUM_OF_IRQ] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint32_t read_ioapic_reg( uint32_t );
static void write_ioapic_reg( uint32_t, uint32_t );
static uint64_t read_redtbl_entry( uint8_t );
static int mask_gsi( uint8_t );
static int unmask_gsi( uint8_t );
void map_redtbl_entry( uint8_t, uint8_t );




void IOAPIC::init( const ACPI::Response &res )
{
    syslog log("IOAPIC::init");

    acpires    = res;
    g_ioBase   = res.ioapic_base;
    g_gsiBase  = res.gsi_base;
    g_ioRegSel = (uint32_t*)(IO_REG_SEL_ADDR);
    g_ioWin    = (uint32_t*)(IO_WIN_ADDR);

    VMM::mapPage((uintptr_t)g_ioBase, (uintptr_t)g_ioBase);
    VMM::mapPage((uintptr_t)g_gsiBase, (uintptr_t)g_gsiBase);
    VMM::mapPage((uintptr_t)g_ioRegSel, (uintptr_t)g_ioRegSel);
    VMM::mapPage((uintptr_t)g_ioWin, (uintptr_t)g_ioWin);

    g_maxRedirectionEntry = ((read_ioapic_reg(IOAPICVER) >> 16) & 0xff) & ~(1 << 7);

    log("A");
    for (uint8_t i=0; i<g_maxRedirectionEntry+1; i++)
    {
        mask_gsi(i);
    }

    log("B");
    size_t count = res.madt.ioapic_iso_count;
    auto  &table = res.madt.ioapic_iso;

    for (uint64_t i=0; i<count; i++)
    {
        map_redtbl_entry(table[i].gsi, table[i].irq_source);
        if (table[i].gsi < 16)
        {
            irq_map[table[i].gsi] = 1;
        }
    }

    log("C");
    for (uint8_t i=0; i<NUM_OF_IRQ; i++)
    {
        if (!irq_map[i])
        {
            map_redtbl_entry(i, i);
        }
    }

    log("Done!");
}

#define LVT_TIMER_REG_OFFSET 0x320


void IOAPIC::maskIRQ( uint8_t irqno )
{
    uint32_t *lvt_timer_reg = (uint32_t*)((uint64_t)(acpires.lapic_base) + LVT_TIMER_REG_OFFSET);

    if (irqno == 0)
    {
        *lvt_timer_reg |= (1 << 16);
    }
    
    else
    {
        size_t count = acpires.madt.ioapic_iso_count;
        auto  &table = acpires.madt.ioapic_iso;
    
        for (uint64_t i=0; i<count; i++)
        {
            if (table[i].irq_source == irqno)
            {
                mask_gsi((uint8_t)(table[i].gsi));
                return;
            }
        }
    }

    mask_gsi(irqno);
}

void IOAPIC::unmaskIRQ( uint8_t irqno )
{
    uint32_t *lvt_timer_reg = (uint32_t*)((uint64_t)(acpires.lapic_base) + LVT_TIMER_REG_OFFSET);

    if (irqno == 0)
    {
        *lvt_timer_reg &= ~(1 << 16);
    }

    else
    {
        size_t count = acpires.madt.ioapic_iso_count;
        auto  &table = acpires.madt.ioapic_iso;
    
        for (uint64_t i=0; i<count; i++)
        {
            if (table[i].irq_source == irqno)
            {
                unmask_gsi((uint8_t)(table[i].gsi));
                return;
            }
        }
    }
    unmask_gsi(irqno);
}



// https://wiki.osdev.org/IOAPIC#Programming_the_I/O_APIC


static uint32_t read_ioapic_reg( uint32_t reg )
{
    *(volatile uint32_t*)g_ioRegSel = reg;
    return *(volatile uint32_t*)g_ioWin;
}

static void write_ioapic_reg( uint32_t reg, uint32_t data )
{
    *(volatile uint32_t*)g_ioRegSel = reg;
    *(volatile uint32_t*)g_ioWin = data;
}


static uint64_t read_redtbl_entry( uint8_t entry )
{
    uint8_t offset = 0x10 + 2*entry;
    uint64_t res = read_ioapic_reg(offset);
    res |= (read_ioapic_reg(offset + 1) << 31);
    return res;
}

static int mask_gsi( uint8_t idx )
{
    uint64_t data = read_redtbl_entry(idx);
    if (idx > g_maxRedirectionEntry)
    {
        return -1;
    }

    uint8_t offset = 0x10 + 2*idx;
    write_ioapic_reg(offset, data | (1 << 16));
    return 0;
}


static int unmask_gsi( uint8_t idx )
{
    uint64_t data = read_redtbl_entry(idx);
    if (idx > g_maxRedirectionEntry)
    {
        return -1;
    }
    uint8_t offset = 0x10 + 2*idx;
    data &= ~(1 << 16);
    write_ioapic_reg(offset, data);
    return 0;
}


void map_redtbl_entry( uint8_t entry, uint8_t vector )
{
    IOAPIC::redirection_entry_t data;

    data.raw_data = read_redtbl_entry(entry);
    data.entry.vector = vector + IDT_EXCEPTIONS;
    data.entry.mask = 1;
    data.entry.destination = 0;

    uint8_t offset = 0x10 + 2*entry;
    write_ioapic_reg(offset, data.registers.lower);
    write_ioapic_reg(offset + 1, data.registers.upper);
}






uint32_t IOAPIC::read( uint32_t reg )
{
    uint32_t volatile *addr = (uint32_t volatile *)g_ioBase;
    addr[0] = (reg & 0xff);
    return addr[4];
}

void IOAPIC::write( uint32_t reg, uint32_t value )
{
    uint32_t volatile *addr = (uint32_t volatile *)g_ioBase;
    addr[0] = (reg & 0xff);
    addr[4] = value;
}

void IOAPIC_setRegister( uint32_t idx, uint32_t value )
{
    IOAPIC::write(0x10 + 2*idx+0, value);
    IOAPIC::write(0x10 + 2*idx+1, uint32_t(uint64_t(value) >> 32));
}



