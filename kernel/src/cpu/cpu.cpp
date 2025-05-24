#include <cpu/cpu.hpp>
#include <kernel/log.hpp>
#include <kmalloc.h>


extern "C"
{
    // extern void cpu_fxsave( uint8_t* );
    // extern void cpu_fxrstor( uint8_t* );

    extern void cpu_set_rsp( uintptr_t );

    extern uint64_t cpu_get_cr3( void );
    extern void cpu_set_cr3( uint64_t );
    
    extern void cpu_enable_sse( void );
    extern void cpu_enable_avx( void );
}



namespace CPU
{
    void setRSP( uintptr_t rsp )
    {
        cpu_set_rsp(rsp);
    }

    uint64_t getCR3()
    {
        return cpu_get_cr3();
    }

    void setCR3( uint64_t cr3 )
    {
        cpu_set_cr3(cr3);
    }

}


#ifdef __SSE__
    void CPU::enableSSE()
    {
        cpu_enable_sse();
    }

    void CPU::fxsave( uint8_t *ptr )
    {
        asm volatile("fxsave64 (%0)" ::"r"((uintptr_t)ptr) : "memory");
    }

    void CPU::fxrstor( uint8_t *ptr )
    {
        asm volatile("fxrstor64 (%0)" ::"r"((uintptr_t)ptr) : "memory");
    }

#else
    void CPU::enableSSE() {  }
    void CPU::fxsave( uint8_t* ) {  }
    void CPU::fxrstor( uint8_t* ) {  }

#endif

