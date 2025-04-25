#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kstackframe.h>
#include <atomic>


namespace idk
{
    struct cpu_state;

}

namespace kernel
{
    class CPU
    {
    private:
        __attribute__((aligned(16)))
        inline static char fxdata[4][512];

    public:
        static void enableSSE( int idx );
        static void fxsave( int idx );
        static void fxrstor( int idx );

        // static void static_init();
        // static void static_fxsave(); 
        // static void static_fxrstor();

        // uint64_t getCR3();
        // void     setCR3( uint64_t );
    };

    inline static std::atomic_bool initialized{false};
}


extern "C"
{
    extern void cpu_ctx_switch( uint8_t *stack );
    // extern void cpu_ctx_load( uint8_t *stack );
}

