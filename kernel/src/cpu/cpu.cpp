#include <cpu/cpu.hpp>
#include <cpu/cpuid.hpp>
#include <kernel/log.hpp>
#include <kmemxx.hpp>


cpu_t::cpu_t( size_t cpuid )
:   self(this), id(cpuid), sched2()
{

};



extern "C"
{
    void cpu_enable_fpu();
    void cpu_enable_sse();
    void cpu_disable_sse();
    void cpu_enable_avx();
    void cpu_enable_xsave();

    void cpu_xsave64(void*);
    void cpu_fxsave64( void* );
    void cpu_fsave( void* );

    void cpu_xrstor64(void*);
    void cpu_fxrstor64( void* );
    void cpu_frstor( void* );

    void cpu_stos8 ( void*, uint8_t, size_t );
    void cpu_stos32( void*, uint32_t, size_t );
    void cpu_stos64( void*, uint64_t, size_t );

    void cpu_movs8 ( void*, const void*, size_t );
    void cpu_movs32( void*, const void*, size_t );
    void cpu_movs64( void*, const void*, size_t );

    // void cpu_getCR( uint64_t *cr0, uint64_t *cr2, uint64_t *cr3, uint64_t *cr4 );
    uint64_t cpu_getXCR0();
    uint64_t cpu_getCR0();
    uint64_t cpu_getCR3();
    uint64_t cpu_getCR4();

    void cpu_setXCR0( uint32_t hi, uint32_t lo );
    void cpu_setCR0( uint64_t );
    void cpu_setCR3( uint64_t );
    void cpu_setCR4( uint64_t );
    void cpu_getTSC( uint32_t *hi, uint32_t *lo );

    void cpu_setMXCSR();

}


#ifdef __SSE__
    #include <immintrin.h>
#endif

static CPU::cpu_features_t *CPU_FEATURES = nullptr;

void CPU_featureCheck2()
{
    const auto &F = *CPU_FEATURES;

    syslog lg("CPU::featureCheck");
    lg("fpu:     %u", F.fpu);
    lg("mmx:     %u", F.mmx);
    lg("sse:     %u", F.sse);
    lg("sse2:    %u", F.sse2);
    lg("sse3:    %u", F.sse3);
    lg("avx:     %u", F.avx);
    lg("avx2:    %u", F.avx2);
    lg("fxsave:  %u", F.fxsave);
    lg("xsave:   %u", F.xsave);
    lg("osxsave: %u", F.osxsave);
}


namespace CPU
{
    // static void (*fpSaveRegsFunc)(void*);
    // static void (*fpLoadRegsFunc)(void*);

    void initFoat()
    {
        static cpu_features_t F;
        kmemset<uint8_t>(&F, 0, sizeof(F));
        CPU_FEATURES = &F;

        uint32_t eax, ebx, ecx, edx;
        __cpuid(0x01, eax, ebx, ecx, edx);

        F.fpu     = bool(edx & CPUID_FEAT_EDX_FPU);
        F.mmx     = bool(edx & CPUID_FEAT_EDX_MMX);
        F.sse     = bool(edx & CPUID_FEAT_EDX_SSE);
        F.sse2    = bool(edx & CPUID_FEAT_EDX_SSE2);
        F.sse3    = bool(ecx & CPUID_FEAT_ECX_SSE3);
        F.avx     = bool(ecx & CPUID_FEAT_ECX_AVX);
        F.fxsave  = bool(ecx & CPUID_FEAT_EDX_FXSR);
        F.xsave   = bool(ecx & CPUID_FEAT_ECX_XSAVE);
        F.osxsave = bool(ecx & CPUID_FEAT_ECX_OSXSAVE);

        // __cpuid(0x07, eax, ebx, ecx, edx);
        // F.avx2 = bool(ebx & bit_AVX2);

        // fpSaveRegsFunc = nullptr;
        // fpLoadRegsFunc = nullptr;

        // if (F.xsave)
        // {
        //     CPU::fpSaveRegsFunc = cpu_xsave64;
        //     CPU::fpLoadRegsFunc = cpu_xrstor64;
        // }

        // else if (F.fxsave)
        // {
        //     CPU::fpSaveRegsFunc = cpu_fxsave64;
        //     CPU::fpLoadRegsFunc = cpu_fxrstor64;
        // }

        // else
        // {
        //     CPU::fpSaveRegsFunc = cpu_fsave;
        //     CPU::fpLoadRegsFunc = cpu_frstor;
        // }

        CPU::enableFloat();
    }

    void enableFloat()
    {
        const auto &F = *CPU_FEATURES;

        if (F.sse)
        {
            cr0_t cr0{getCR0()};
            cr0.MP = 1;
            cr0.EM = 0;
            cr0.TS = 0;
            cr0.ET = 1;
            cr0.NE = 1;
            setCR0(cr0.qword);

            cr4_t cr4{getCR4()};
            cr4.OSFXSR     = 1;
            cr4.OSXMMEXCPT = 1;
            cr4.OSXSAVE    = (F.xsave) ? 1 : 0;
            setCR4(cr4.qword);

            // if (F.xsave && F.avx)
            // {
            //     cpu_enable_xsave();
            //     cpu_enable_avx();
            // }
        }
    }

    void disableFloat()
    {
        
    }

    void fpSaveRegs( void *p )
    {
        // fpSaveRegsFunc(p);
        if (CPU_FEATURES->fxsave)
            cpu_fxsave64(p);
    }

    void fpLoadRegs( void *p )
    {
        // fpSaveRegsFunc(p);
        if (CPU_FEATURES->fxsave)
            cpu_fxrstor64(p);
    }


// #else
    // void enableFloat() {  }
    // void disableFloat() {  }
    // void fxsave ( void* ) {  }
    // void fxrstor( void* ) {  }

// #endif

    void stos8 ( void *d,  uint8_t v, size_t n ) { cpu_stos8(d, v, n);  }
    void stos32( void *d, uint32_t v, size_t n ) { cpu_stos32(d, v, n); }
    void stos64( void *d, uint64_t v, size_t n ) { cpu_stos64(d, v, n); }

    void movs8 ( void *d, const void *s, size_t n ) { cpu_movs8(d, s, n);  }
    void movs32( void *d, const void *s, size_t n ) { cpu_movs32(d, s, n); }
    void movs64( void *d, const void *s, size_t n ) { cpu_movs64(d, s, n); }

    uint64_t getCR0() { return cpu_getCR0(); }
    uint64_t getCR3() { return cpu_getCR3(); }
    uint64_t getCR4() { return cpu_getCR4(); }

    void setCR0( uint64_t x ) { cpu_setCR0(x); }
    void setCR3( uint64_t x ) { cpu_setCR3(x); }
    void setCR4( uint64_t x ) { cpu_setCR4(x); }

    uint64_t getTSC()
    {
        uint32_t hi, lo;
        // cpu_getTSC(&hi, &lo);
        asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
        
        return ((uint64_t)hi << 32) | (uint64_t)lo;
    }

}

