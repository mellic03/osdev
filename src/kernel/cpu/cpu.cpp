#include "cpu.hpp"
#include <kstackframe.h>


extern "C"
{
    extern void __cpu_enable_SSE(void);

    extern uint64_t cpu_get_cr3(void);
    extern void     cpu_set_cr3(uint64_t);
    // extern void cpu_ctx_load( kstackframe* );
    // extern void cpu_ctx_save( kstackframe* );
}


void
kernel::CPU::enableSSE( int idx )
{
    __cpu_enable_SSE();
    CPU::fxsave(idx);
}


void
kernel::CPU::fxsave( int idx )
{
    asm volatile("fxsave %0 " : : "m"(CPU::fxdata[idx]));
}

void
kernel::CPU::fxrstor( int idx )
{
    asm volatile("fxrstor %0 " : : "m"(CPU::fxdata[idx]));
}


// void
// idk::CPU::static_init()
// {
//     // #ifdef __libk_sse
//     __cpu_enable_SSE();
//     CPU::static_fxsave();
//     // #endif
// }

// void
// idk::CPU::static_fxsave()
// {
//     asm volatile("fxsave %0 " : : "m"(CPU::static_fxdata));
// }

// void
// idk::CPU::static_fxrstor()
// {
//     asm volatile("fxrstor %0" : : "m"(CPU::static_fxdata));
// }


// uint64_t
// idk::CPU::getCR3()
// {
//     return cpu_get_cr3();
// }


// void
// idk::CPU::setCR3( uint64_t value )
// {
//     return cpu_set_cr3(value);
// }