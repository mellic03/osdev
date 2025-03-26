#include "cpu.hpp"
#include <kstackframe.h>


extern "C"
{
    extern void __cpu_enable_SSE(void);

    extern uint64_t __cpu_get_cr3(void);
    extern void     __cpu_set_cr3(uint64_t);
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


uint64_t
idk::CPU::getCR3()
{
    return __cpu_get_cr3();
}


void
idk::CPU::setCR3( uint64_t value )
{
    return __cpu_set_cr3(value);
}