#pragma once
#include <kdef.h>
#include <stdint.h>
#include <atomic>
#include <mutex>

#include "tss.hpp"
#include "gdt.hpp"
#include "idt.hpp"
#include <smp/smp.hpp>
#include <sys/process.hpp>

struct kthread_t;
struct ThreadScheduler;


struct cpu_t
{
    cpu_t     *self;
    uint64_t   id;

    std::atomic_uint64_t m_ticks{0};
    // ThreadScheduler sched;
    knl::Sched      sched2;

    uint64_t   syscall_no;
    uintptr_t  syscall_req;
    uintptr_t  syscall_res;

    cpu_t(): sched2() {  };
    cpu_t( size_t cpuid );
};



namespace CPU
{
    static constexpr uint16_t GDT_OFFSET_KERNEL_CODE = 0x08;
    static constexpr uint16_t GDT_OFFSET_KERNEL_DATA = 0x10;

    static constexpr uint64_t MSR_EFER           = 0xC0000080;
    static constexpr uint64_t MSR_PAT            = 0x0277;
    static constexpr uint32_t MSR_FS_BASE        = 0xC0000100;
    static constexpr uint32_t MSR_GS_BASE        = 0xC0000101;
    static constexpr uint32_t MSR_GS_KERNEL_BASE = 0xC0000102;


    struct cpu_features_t
    {
        bool fpu, mmx;
        bool sse, sse2, sse3;
        bool avx, avx2;
        bool xsave, osxsave;
    };

    void enableFloat();
    void fxsave( void *p );
    void fxrstor( void *p );

    void createGDT();
    void installGDT();
    void createGDT( uint64_t *gdtbase, gdt_ptr_t *gdtr );
    void installGDT( gdt_ptr_t *gdtr );

    void createIDT();
    void installIDT();
    // void createIDT( idt_entry_t *idtbase, idt_ptr_t *idtptr );
    // void installIDT( idt_ptr_t idtptr );

    void installISR( uint8_t isrno, isrHandlerFn fn );
    void installIRQ( uint8_t irqno, irqHandlerFn fn );

    inline void nop() { asm volatile ("nop"); }
    inline void cli() { asm volatile ("cli"); }
    inline void sti() { asm volatile ("sti"); }
    inline void hlt() { asm volatile ("hlt"); }
    inline void cli_hlt() { asm volatile ("cli; hlt"); }
    inline void hcf() { while (true) { asm volatile ("cli; hlt"); } }

    void     stos8 ( void *dst,  uint8_t value, size_t count );
    void     stos32( void *dst, uint32_t value, size_t count );
    void     stos64( void *dst, uint64_t value, size_t count );
    void     movs8 ( void *dst, const void *src, size_t count );
    void     movs32( void *dst, const void *src, size_t count );
    void     movs64( void *dst, const void *src, size_t count );

    uint64_t getCR0();
    uint64_t getCR3();
    uint64_t getCR4();
    void     setCR0( uint64_t );
    void     setCR3( uint64_t );
    void     setCR4( uint64_t );
    uint64_t getTSC();

    inline uint64_t rdmsr( uint32_t msr )
    {
        uint32_t lo, hi;
        asm volatile("rdmsr" : "=a"(lo), "=d"(hi) : "c"(msr));
        return (uint64_t)lo | ((uint64_t)hi << 32);
    }

    inline void wrmsr( uint32_t msr, uint64_t value )
    {
        asm volatile ("wrmsr" : : "c"(msr), "a"(value & 0xFFFFFFFF), "d"(value >> 32));
    }

    inline void flushTLB( uintptr_t addr )
    {
        asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
    }

    inline uint64_t getRFLAGS()
    {
        uint64_t flags;
        asm volatile("pushf\n\t"
                     "pop %0"
                     : "=g"(flags));
        return flags;
    }

    inline void setRFLAGS( uint64_t rflags )
    {
        asm volatile (
            "pushfq\n\t"
            "pop %0"
            : "=r"(rflags)
        );
    }

    // __attribute__((always_inline))
    // inline bool knl::checkInterrupts()
    // {
    //     volatile unsigned long flags;
    //     asm volatile("pushfq;"
    //                  "pop %0;"
    //                 : "=rm"(flags)::"memory", "cc");
    //     return (flags & 0x200) != 0;
    // }


    union cr0_t
    {
        uint64_t qword;

        struct
        {
            uint32_t lo;
            uint32_t hi;
        };

        struct
        {
            // uint8_t protectedModeEnable     : 1; // 0	PE	Protected Mode Enable
            // uint8_t monitorCoprocessor      : 1; // 1	MP	Monitor co-processor
            // uint8_t x87FPUEmulation         : 1; // 2	EM	x87 FPU Emulation
            // uint8_t taskSwitched            : 1; // 3	TS	Task switched
            // uint8_t extensionType           : 1; // 4	ET	Extension type
            // uint8_t numericError            : 1; // 5	NE	Numeric error

            uint8_t  PE     : 1;    // 0    Protected Mode Enable
            uint8_t  MP     : 1;    // 1    Monitor Co-Processor
            uint8_t  EM     : 1;    // 2    x87 FPU Emulation
            uint8_t  TS     : 1;    // 3    Task Switched
            // uint8_t  ET     : 1;    // 4    Extension Type
            // uint8_t  NE     : 1;    // 5    Numeric Error
            // uint16_t resv0  : 10;   // 6-15 Reserved
            // uint8_t  WP     : 1;    // 16   Write Protect
            // uint8_t  resv1  : 1;    // 17   Reserved
            // uint8_t  AM     : 1;    // 18   Alignment Mask
            // uint16_t resv2  : 10;   // 19-28 Reserved
            // uint8_t  NW     : 1;    // 29   Not-Write Through
            // uint8_t  CD     : 1;    // 30   Cache Disable
            // uint8_t  PG     : 1;    // 31   Paging
            // uint32_t resv3  : 32;
        };
    };

    union xcr0_t
    {
        uint64_t qword;
    
        struct
        {
            uint32_t lo;
            uint32_t hi;
        };

        struct
        {
            uint8_t X87       : 1; // x87 FPU/MMX support (must be 1)
            uint8_t SSE       : 1; // XSAVE support for MXCSR and XMM registers
            uint8_t AVX       : 1; // AVX enabled and XSAVE support for upper halves of YMM registers
            uint8_t BNDREG    : 1; // MPX enabled and XSAVE support for BND0-BND3 registers
            uint8_t BNDCSR    : 1; // MPX enabled and XSAVE support for BNDCFGU and BNDSTATUS registers
            uint8_t opmask    : 1; // AVX-512 enabled and XSAVE support for opmask registers k0-k7
            uint8_t ZMM_Hi256 : 1; // AVX-512 enabled and XSAVE support for upper halves of lower ZMM registers
            uint8_t Hi16_ZMM  : 1; // AVX-512 enabled and XSAVE support for upper ZMM registers
            uint8_t PKRU      : 1; // XSAVE support for PKRU register 
        };
        
    };


    union cr4_t
    {
        uint64_t qword;

        struct
        {
            uint32_t lo;
            uint32_t hi;
        };

        struct
        {
            uint8_t VME         : 1; // Virtual 8086 Mode Extensions
            uint8_t PVI         : 1; // Protected-mode Virtual Interrupts
            uint8_t TSD         : 1; // Time Stamp Disable
            uint8_t DE          : 1; // Debugging Extensions
            uint8_t PSE         : 1; // Page Size Extension
            uint8_t PAE         : 1; // Physical Address Extension
            uint8_t MCE         : 1; // Machine Check Exception
            uint8_t PGE         : 1; // Page Global Enabled
            uint8_t PCE         : 1; // Performance-Monitoring Counter enable
            uint8_t OSFXSR      : 1; // Operating system support for FXSAVE and FXRSTOR instructions
            uint8_t OSXMMEXCPT  : 1; // Operating System Support for Unmasked SIMD Floating-Point Exceptions
            uint8_t UMIP        : 1; // User-Mode Instruction Prevention (if set, #GP on SGDT, SIDT, SLDT, SMSW, and STR instructions when CPL > 0)
            uint8_t LA57        : 1; // 57-bit linear addresses (if set, the processor uses 5-level paging otherwise it uses uses 4-level paging)
            uint8_t VMXE        : 1; // Virtual Machine Extensions Enable
            uint8_t SMXE        : 1; // Safer Mode Extensions Enable
            uint8_t resv0       : 1;
            uint8_t FSGSBASE    : 1; // Enables the instructions RDFSBASE, RDGSBASE, WRFSBASE, and WRGSBASE
            uint8_t PCIDE       : 1; // PCID Enable
            uint8_t OSXSAVE     : 1; // XSAVE and Processor Extended States Enable
            uint8_t resv1       : 1;
            uint8_t SMEP        : 1; // Supervisor Mode Execution Protection Enable
            uint8_t SMAP        : 1; // Supervisor Mode Access Prevention Enable
            uint8_t PKE         : 1; // Protection Key Enable
            uint8_t CET         : 1; // Control-flow Enforcement Technology
            uint8_t PKS         : 1; // Enable Protection Keys for Supervisor-Mode Pages
        };
    };



    union mxcsr_t
    {
        uint32_t dword;

        struct
        {
            uint8_t IE  : 1; // Invalid Operation Flag
            uint8_t DE  : 1; // Denormal Flag
            uint8_t ZE  : 1; // Divide-by-Zero Flag
            uint8_t OE  : 1; // Overflow Flag
            uint8_t UE  : 1; // Underflow Flag
            uint8_t PE  : 1; // Precision Flag
            uint8_t DAZ : 1; // Denormals Are Zeros
            uint8_t IM  : 1; // Invalid Operation Mask
            uint8_t DM  : 1; // Denormal Operation Mask
            uint8_t ZM  : 1; // Divide-by-Zero Mask
            uint8_t OM  : 1; // Overflow Mask
            uint8_t UM  : 1; // Underflow Mask
            uint8_t PM  : 1; // Precision Mask
            uint8_t RC  : 1; // Rounding Control
            uint8_t FZ  : 1; // Flush to Zero
        };
    };

    struct fxstate_t
    {
        uint16_t fcw;           // FPU Control Word
        uint16_t fsw;           // FPU Status Word
        uint8_t  ftw;           // FPU Tag Words
        uint8_t  zero;          // Literally just contains a zero
        uint16_t fop;           // FPU Opcode
        uint64_t rip;
        uint64_t rdp;
        uint32_t mxcsr;         // SSE Control Register
        uint32_t mxcsrMask;     // SSE Control Register Mask
        uint8_t  st[8][16];     // FPU Registers, Last 6 bytes reserved
        uint8_t  xmm[16][16];   // XMM Registers
    } __attribute__((packed));

}

















