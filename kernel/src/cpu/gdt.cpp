#include <cpu/cpu.hpp>
#include <cpu/gdt.hpp>

extern "C"
{
    extern void GDT64_Install( gdt_ptr_t* );
    // extern void cpu_load_gdt( gdt_ptr_t* );
    // extern void cpu_flush_gdt( void );
}

static uint64_t  default_GDT[5];
static gdt_ptr_t default_GDTR;
// static tss_t     default_TSS;


static void
fill_entry( uint64_t *gdtbase, int idx, uint64_t base, uint64_t limit, uint8_t access_byte, uint8_t flags )
{
    uint8_t *desc = (uint8_t*)(gdtbase + idx);

    // Encode the limit
    desc[0] =  limit & 0xFF;
    desc[1] = (limit >> 8) & 0xFF;
    desc[6] = (limit >> 16) & 0x0F;

    // // Encode the base
    desc[2] =  base & 0xFF;
    desc[3] = (base >> 8) & 0xFF;
    desc[4] = (base >> 16) & 0xFF;
    desc[7] = (base >> 24) & 0xFF;

    // // Encode the access byte
    desc[5] = access_byte;

    // // Encode the flags
    desc[6] |= (flags << 4);
}



void CPU::createGDT( uint64_t *gdtbase, gdt_ptr_t *gdtr)
{
    // https://wiki.osdev.org/GDT_Tutorial#How_to_Set_Up_The_GDT
    fill_entry(gdtbase, 0, 0, 0, 0, 0);
    fill_entry(gdtbase, 1, 0, 0xFFFFF, 0x9A, 0xA);        // Kernel mode code segment
    fill_entry(gdtbase, 2, 0, 0xFFFFF, 0x92, 0xC);        // Kernel mode data segment
    fill_entry(gdtbase, 3, 0xFFFFF, 0xF00000, 0xFA, 0xA); // User mode code segment
    fill_entry(gdtbase, 4, 0xFFFFF, 0xF00000, 0xF2, 0xC); // User mode data segment

    // uint64_t tssBase = (uint64_t)TSS; // Address of your TSS structure
    // uint32_t tssLimit = sizeof(tss_t) - 1;
    // fill_entry(default_GDT, 5, tssBase & 0xFFFFFF, tssLimit, 0x89, 0x0); // TSS (low 32 bits of base)
    // fill_entry(default_GDT, 6, (tssBase >> 32) & 0xFFFFFFFF, 0, 0, 0);   // TSS (high 32 bits of base)
    size_t numDescriptors = 5;

    *gdtr = {
        .limit = (uint16_t)(numDescriptors * sizeof(uint64_t) - 1),
        .base  = (uint64_t)(gdtbase)
    };
}


void CPU::createGDT()
{
    // https://wiki.osdev.org/GDT_Tutorial#How_to_Set_Up_The_GDT
    fill_entry(default_GDT, 0, 0, 0, 0, 0);
    fill_entry(default_GDT, 1, 0, 0xFFFFF, 0x9A, 0xA); // Kernel mode code segment
    fill_entry(default_GDT, 2, 0, 0xFFFFF, 0x92, 0xC); // Kernel mode data segment
    fill_entry(default_GDT, 3, 0xFFFFF, 0xF00000, 0xFA, 0xA); // User mode code segment
    fill_entry(default_GDT, 4, 0xFFFFF, 0xF00000, 0xF2, 0xC); // User mode data segment


    // uint64_t tssBase = (uint64_t)(&default_TSS); // Address of your TSS structure
    // uint32_t tssLimit = sizeof(tss_t) - 1;
    // fill_entry(default_GDT, 5, tssBase & 0xFFFFFF, tssLimit, 0x89, 0x0); // TSS (low 32 bits of base)
    // fill_entry(default_GDT, 6, (tssBase >> 32) & 0xFFFFFFFF, 0, 0, 0);   // TSS (high 32 bits of base)
    size_t numDescriptors = 5;

    default_GDTR = {
        .limit = (uint16_t)(numDescriptors * sizeof(uint64_t) - 1),
        .base  = (uint64_t)(default_GDT)
    };

}


void CPU::installGDT()
{
    GDT64_Install(&default_GDTR);
}


void CPU::installGDT( gdt_ptr_t *gdtr )
{
    GDT64_Install(gdtr);
}

