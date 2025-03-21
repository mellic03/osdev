#include "cpu.hpp"
#include <kstackframe.h>


extern "C"
{
    extern void __cpu_enable_SSE(void);
    // extern void cpu_ctx_load( kstackframe* );
    // extern void cpu_ctx_save( kstackframe* );
}


void
idk::CPU::init()
{
    #ifdef __libk_sse
        __cpu_enable_SSE();
        fxsave();
    #endif
}



void
idk::CPU::fxsave()
{
    asm volatile("fxsave %0 " : : "m"(m_fxsave));
}


void
idk::CPU::fxrstor()
{
    asm volatile("fxrstor %0" : : "m"(m_fxsave));
}

