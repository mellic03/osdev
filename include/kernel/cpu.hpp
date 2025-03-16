#pragma once

namespace idk
{
    struct CPU
    {
    private:
        __attribute__((aligned(16)))
        char m_fxsave[512];

    public:
        void fxsave(); 
        void fxrstor();
        void enableSSE();
    };
}
