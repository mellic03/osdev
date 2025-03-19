#include <kernel/cpu.hpp>

extern "C"
{
    extern void __cpu_enable_SSE(void);
}


void
idk::CPU::init()
{
    #ifdef IDKERNEL_SSE
        __cpu_enable_SSE();
        fxsave();
    #endif

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




// void asm_jmp( void *addr )
// {
//     void (*yolo)(void) = (void (*)())addr;
//     yolo();
// }
