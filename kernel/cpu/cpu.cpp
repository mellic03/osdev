#include <kernel/cpu.hpp>

extern "C"
{
    extern void __cpu_enable_SSE(void);
}

   
void
idk::CPU::fxsave()
{
    asm volatile("fxsave %0 "::"m"(m_fxsave));
}


void
idk::CPU::fxrstor()
{
    asm volatile("fxrstor %0" : : "m"(m_fxsave));
}


void
idk::CPU::enableSSE()
{
    __cpu_enable_SSE();
    fxsave();
}
