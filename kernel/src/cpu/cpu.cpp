#include <cpu/cpu.hpp>
#include <kernel/log.hpp>
#include <kmalloc.h>


extern "C"
{
    // extern void cpu_fxsave( uint8_t* );
    // extern void cpu_fxrstor( uint8_t* );

    extern uint64_t cpu_get_cr3( void );
    extern void cpu_set_cr3( uint64_t );
    
    extern void cpu_load_gdt( gdt_ptr_t* );
    extern void cpu_flush_gdt( void );

    extern void cpu_enable_sse( void );
    extern void cpu_enable_avx( void );
}

namespace CPU
{
    uint64_t getCR3()
    {
        return cpu_get_cr3();
    }

    void setCR3( uint64_t cr3 )
    {
        cpu_set_cr3(cr3);
    }

    // auto getCR3 = cpu_get_cr3;
    // auto setCR3 = cpu_set_cr3;
}


#ifdef __SSE__
    void CPU::enableSSE()
    { cpu_enable_sse(); }

    void CPU::fxsave( uint8_t *dst )
    { asm volatile("fxsave %0 " : : "m"(dst)); }

    void CPU::fxrstor( uint8_t *src )
    { asm volatile("fxrstor %0 " : : "m"(src)); }

#else
    void CPU::enableSSE() {  }
    void CPU::fxsave( uint8_t* ) {  }
    void CPU::fxrstor( uint8_t* ) {  }

#endif



#define numDescriptors 5
static uint64_t  globalDescriptors[16];
static gdt_ptr_t globalDescPointer;


static void
fill_entry( int idx, uint64_t base, uint64_t limit, uint8_t access_byte, uint8_t flags )
{
    uint8_t *desc = (uint8_t*)(&globalDescriptors[idx]);

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


void CPU::createGDT()
{
    // https://wiki.osdev.org/GDT_Tutorial#How_to_Set_Up_The_GDT
    fill_entry(0, 0, 0, 0, 0);
    fill_entry(1, 0, 0xFFFFF, 0x9A, 0xA); // Kernel mode code segment
    fill_entry(2, 0, 0xFFFFF, 0x92, 0xC); // Kernel mode data segment
    fill_entry(3, 0xFFFFF, 0xF00000, 0xFA, 0xA); // User mode code segment
    fill_entry(4, 0xFFFFF, 0xF00000, 0xF2, 0xC); // User mode data segment

    globalDescPointer = {
        .limit = (uint16_t)(numDescriptors * sizeof(uint64_t) - 1),
        .base  = (uint64_t)(&globalDescriptors[0])
    };

    // syslog log("CPU::createGDT");
    // for (int i=0; i<numDescriptors; i++)
    // {
    //     log("GDT[%d]: 0x%lx", i, globalDescriptors[i]);
    // }

    // log("gdtr.limit: %lu",   globalDescPointer.limit);
    // log("gdtr.base:  0x%lx", globalDescPointer.base);
}



void CPU::installGDT()
{
    cpu_load_gdt(&globalDescPointer);
    cpu_flush_gdt();
}


