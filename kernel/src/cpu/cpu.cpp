#include <cpu/cpu.hpp>
#include <kernel/log.hpp>
#include <kmalloc.h>


cpu_t::cpu_t( size_t cpuid )
:   self(this), id(cpuid), sched()
{
    CPU::wrmsr(CPU::MSR_GS_BASE, (uint64_t)this);
    CPU::wrmsr(CPU::MSR_KERNEL_GS_BASE, (uint64_t)this);
};



extern "C"
{
    void cpu_enable_fpu( void );
    void cpu_enable_sse( void );
    void cpu_disable_sse( void );
    void cpu_enable_avx( void );
    void cpu_enable_xsave( void );
    void cpu_fxsave64( uintptr_t );
    void cpu_fxrstor64( uintptr_t );

    void cpu_stos8 ( void*, uint8_t, size_t );
    void cpu_stos32( void*, uint32_t, size_t );
    void cpu_stos64( void*, uint64_t, size_t );

    void cpu_movs8 ( void*, const void*, size_t );
    void cpu_movs32( void*, const void*, size_t );
    void cpu_movs64( void*, const void*, size_t );

    uint64_t cpu_getCR3( void );
    void cpu_setCR3( uint64_t );
    void cpu_getTSC( uint32_t *hi, uint32_t *lo );
}


#ifdef __SSE__
    #include <immintrin.h>
#endif


namespace CPU
{
    void featureCheck()
    {
        unsigned int eax, ebx, ecx, edx;
        __asm__ volatile("cpuid"
                        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                        : "a"(0x01));
        syslog log("CPU::featureCheck");

        if (!(edx & (1 << 12)))
            log("Write combining unavailable");
        else
            log("Write combining available");
    }

    #ifdef __SSE__
        void enableFloat()
        {
            cpu_enable_fpu();
            cpu_enable_sse();
            #ifdef __AVX__
                #error AVX not supported!
            #endif
        }

        void disableFloat() {  }
        void fxsave ( void *p ) { cpu_fxsave64((uintptr_t)p); }
        void fxrstor( void *p ) { cpu_fxrstor64((uintptr_t)p); }

    #else
        void enableFloat() {  }
        void disableFloat() {  }
        void fxsave ( void* ) {  }
        void fxrstor( void* ) {  }

    #endif

    void stos8 ( void *d,  uint8_t v, size_t n ) { cpu_stos8(d, v, n);  }
    void stos32( void *d, uint32_t v, size_t n ) { cpu_stos32(d, v, n); }
    void stos64( void *d, uint64_t v, size_t n ) { cpu_stos64(d, v, n); }

    void movs8 ( void *d, const void *s, size_t n ) { cpu_movs8(d, s, n);  }
    void movs32( void *d, const void *s, size_t n ) { cpu_movs32(d, s, n); }
    void movs64( void *d, const void *s, size_t n ) { cpu_movs64(d, s, n); }

    uint64_t getCR3() { return cpu_getCR3(); }
    void setCR3( uint64_t cr3 ) { cpu_setCR3(cr3); }

    uint64_t getTSC()
    {
        uint32_t hi, lo;
        // cpu_getTSC(&hi, &lo);
        asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
        
        return ((uint64_t)hi << 32) | (uint64_t)lo;
    }

}

