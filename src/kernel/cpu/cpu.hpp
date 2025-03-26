#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kstackframe.h>


namespace idk
{
    struct cpu_state;

    struct CPU
    {
    private:
        __attribute__((aligned(16)))
        char m_fxsave[512];

    public:
        void init();
        void fxsave(); 
        void fxrstor();

        uint64_t getCR3();
        void     setCR3( uint64_t );
    };

}


extern "C"
{
    extern void cpu_ctx_switch( uint8_t *stack );
    // extern void cpu_ctx_load( uint8_t *stack );
}

