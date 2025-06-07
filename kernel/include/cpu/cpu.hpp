#pragma once
#include <kdef.h>
#include <stdint.h>
#include <cpuid.h>
#include <atomic>
#include <mutex>

#include "tss.hpp"
#include "gdt.hpp"
#include "idt.hpp"
#include "scheduler.hpp"

struct kthread_t;
struct ThreadScheduler;


static constexpr uint64_t MSR_PAT          = 0x0277;
static constexpr uint64_t MSR_FS_BASE      = 0xC0000100;
static constexpr uint64_t MSR_GS_BASE      = 0xC0000101;
static constexpr uint64_t MSR_KERN_GS_BASE = 0xC0000102;
static constexpr uint64_t MSR_EFER         = 0xC0000080;

/* @see https://wiki.osdev.org/SYSCALL#AMD:_SYSCALL.2FSYSRET */
/* Ring 0 and Ring 3 Segment bases, as well as SYSCALL EIP.
 * Low 32 bits = SYSCALL EIP, bits 32-47 are kernel segment base, bits 48-63 are
 * user segment base.
 */
static constexpr uint64_t MSR_STAR =   0xC0000081;
/* The kernel's RIP SYSCALL entry for 64 bit software */
static constexpr uint64_t MSR_LSTAR =   0xC0000082;
/* The kernel's RIP for SYSCALL in compatibility mode */
static constexpr uint64_t MSR_CSTAR =   0xC0000083;
/* The low 32 bits are the SYSCALL flag mask. If a bit in this is set, the
 * corresponding bit in rFLAGS is cleared
 */
static constexpr uint64_t MSR_SFMASK =   0xC0000084;






struct cpu_t
{
    cpu_t     *self;
    uint64_t   id;

    std::atomic_uint64_t m_msecs{0};
    ThreadScheduler sched;

    uint8_t    yldrsn;
    uint64_t   syscall_no;
    uintptr_t  syscall_req;
    uintptr_t  syscall_res;

    cpu_t(): sched() {  };
    cpu_t( size_t cpuid );
};


namespace SMP
{
    // extern uint8_t all_cpus[];
    static constexpr size_t max_cpus = 8;
    extern size_t num_cpus;

    cpu_t     *get_cpu( uint32_t id );

    bool       is_bsp();
    uint64_t   bsp_id();

    cpu_t     *this_cpu();
    uint64_t   this_cpuid();
    ksched_t  *this_sched();

    kthread_t *this_thread();
    uint64_t   this_tid();
}



namespace CPU
{
    static constexpr uint16_t GDT_OFFSET_KERNEL_CODE = 0x08;
    static constexpr uint16_t GDT_OFFSET_KERNEL_DATA = 0x10;
    static constexpr uint32_t MSR_FS_BASE        = 0xC0000100;
    static constexpr uint32_t MSR_GS_BASE        = 0xC0000101;
    static constexpr uint32_t MSR_KERNEL_GS_BASE = 0xC0000102;

    void featureCheck();

    void enableFloat();
    void fxsave( void *p );
    void fxrstor( void *p );

    void createGDT();
    void installGDT();
    void createGDT( uint64_t *gdtbase, gdt_ptr_t *gdtr );
    void installGDT( gdt_ptr_t *gdtr );

    void createIDT();
    void installIDT();
    void createIDT( idt_entry_t *idtbase, idt_ptr_t *idtptr );
    void installIDT( idt_ptr_t idtptr );

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
    void     movs8( void *dst, const void *src, size_t count );
    void     movs32( void *dst, const void *src, size_t count );
    void     movs64( void *dst, const void *src, size_t count );

    void     setRSP( uintptr_t );
    uint64_t getCR3();
    void     setCR3( uint64_t );
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


    // static inline void movsl( void *dst, const void *src, size_t count )
    // {
    //     asm volatile("rep movsl" : "+D"(dst), "+S"(src), "+c"(count) : : "memory");
    // }

    // static inline void movsq( void *dst, const void *src, size_t count )
    // {
    //     asm volatile("rep movsq" : "+D"(dst), "+S"(src), "+c"(count) : : "memory");
    // }

}
