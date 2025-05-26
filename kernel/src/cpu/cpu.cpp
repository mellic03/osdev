#include <cpu/cpu.hpp>
#include <kernel/log.hpp>
#include <kmalloc.h>


extern "C"
{
    extern uint64_t CPU_GetCR3( void );
    extern void CPU_SetCR3( uint64_t );
    extern void cpu_enable_sse( void );
}



cpu_t::cpu_t( size_t cpuid )
:   self(this), id(cpuid), sched(*this)
{
    CPU::wrmsr(CPU::MSR_GS_BASE, (uint64_t)this);
    CPU::wrmsr(CPU::MSR_KERNEL_GS_BASE, (uint64_t)this);
};


namespace CPU
{
    uint64_t getCR3()
    {
        return CPU_GetCR3();
    }

    void setCR3( uint64_t cr3 )
    {
        CPU_SetCR3(cr3);
    }

}


// #ifdef __SSE__
    void CPU::enableSSE()
    {
        cpu_enable_sse();
    }

    void CPU::fxsave( uint8_t *ptr )
    {
        asm volatile("fxsave64 (%0)" ::"r"(ptr) : "memory");
    }

    void CPU::fxrstor( uint8_t *ptr )
    {
        asm volatile("fxrstor64 (%0)" ::"r"(ptr) : "memory");
    }

// #else
    // void CPU::enableSSE() {  }
    // void CPU::fxsave( uint8_t* ) {  }
    // void CPU::fxrstor( uint8_t* ) {  }

// #endif

