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

    };

}

extern "C"
{
    extern void cpu_ctx_load( kstackframe* );
    extern void cpu_ctx_save( kstackframe* );
}

