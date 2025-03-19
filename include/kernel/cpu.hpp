#pragma once

#include <stddef.h>
#include <stdint.h>


namespace idk
{
    struct cpu_state;

    struct CPU
    {
    private:
        __attribute__((aligned(16)))
        char m_fxsave[512];
        void fxsave(); 
        void fxrstor();

    public:
        void init();
    };

}

