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


static constexpr uint64_t MSR_PAT =   0x0277;

static constexpr uint64_t MSR_FS_BASE       =   0xC0000100;
static constexpr uint64_t MSR_GS_BASE       =   0xC0000101;
static constexpr uint64_t MSR_KERN_GS_BASE  =   0xC0000102;

static constexpr uint64_t MSR_EFER =   0xC0000080;

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

    ThreadScheduler sched;

    uint8_t    yldrsn;
    uint64_t   syscall_no;
    uintptr_t  syscall_req;
    uintptr_t  syscall_res;

    tss_t       tss __attribute__((aligned(16)));
    uint64_t    gdt[5];
    gdt_ptr_t   gdtPtr;

    idt_entry_t idtentries[256];
    idt_ptr_t   idtPtr;

    cpu_t(): sched(*this) {  };
};


namespace SMP
{
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

    void createGDT();
    void installGDT();
    void createGDT( uint64_t *gdtbase, gdt_ptr_t *gdtr, tss_t *TSS );
    void installGDT( gdt_ptr_t *gdtr );

    void clearIDT();
    void createIDT();
    void installIDT();
    void createIDT( idt_entry_t *idtbase, idt_ptr_t *idtptr );
    void installIDT( idt_ptr_t idtptr );

    void installISR( uint8_t isrno, isrHandlerFn fn );
    void installIRQ( uint8_t irqno, irqHandlerFn fn );

}





#include <kernel/interrupt.hpp>

namespace CPU
{
    void enableSSE();
    void fxsave( uint8_t *dst );
    void fxrstor( uint8_t *src );

    static constexpr uint32_t MSR_FS_BASE        = 0xC0000100;
    static constexpr uint32_t MSR_GS_BASE        = 0xC0000101;
    static constexpr uint32_t MSR_KERNEL_GS_BASE = 0xC0000102;

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

    uint64_t getCR3();
    void     setCR3( uint64_t );

    /**
     * Install a new pagetable by changing the CR3 value. Address must be divisable
     * by pagesize. The address must be physical.
     */
    static inline void setPML4( uint64_t pagetable_address )
    {
        __asm__ volatile("mov cr3, rax"
                        :
                        : "a"(pagetable_address & 0xFFFFFFFFFFFFF000ULL));
    }
    
    /**
     * Gets the physical address of installed current page table.
     */
    static inline uint64_t getPML4()
    {
        uint64_t cr3;
        __asm__ volatile("mov rax, cr3" : "=a"(cr3));
        return cr3 & 0xFFFFFFFFFFFFF000ULL;
    }


    // inline uint64_t getRFLAGS()
    // {
    //     uint64_t rflags;
    //     __asm__ volatile (
    //         "pushfq\n"          // Push the RFLAGS register onto the stack
    //         "popq %0\n"         // Pop the value from the stack into rflags
    //         : "=r"(rflags)      // Output operand
    //     );
    //     return rflags;
    // }
    inline uint64_t getRFLAGS()
    {
        uint64_t flags;
        asm volatile("pushf\n\t"
                     "pop %0"
                     : "=g"(flags));
        return flags;
    }

    // inline void setRFLAGS( uint64_t rflags )
    // {
    //     asm volatile (
    //         "pushfq\n\t"
    //         "pop %0"
    //         : "=r"(rflags)
    //     );
    // }
    
    inline void cli() { asm volatile ("cli"); }
    inline void sti() { asm volatile ("sti"); }


    __attribute__((always_inline))
    inline bool checkInterrupts()
    {
        volatile unsigned long flags;
        asm volatile("pushfq;"
                    "pop %0;"
                    : "=rm"(flags)::"memory", "cc");
        return (flags & 0x200) != 0;
    }


    static inline uint64_t getTCS()
    {
        uint32_t timestamp_low, timestamp_high;
        asm volatile("rdtsc" : "=a"(timestamp_low), "=d"(timestamp_high));
        return ((uint64_t)timestamp_high << 32) | ((uint64_t)timestamp_low);
    }


    inline void setLocal( cpu_t *val )
    {
        val->self = val;
        asm volatile("wrmsr" ::"a"((uintptr_t)val & 0xFFFFFFFF) /*Value low*/,
                    "d"(((uintptr_t)val >> 32) & 0xFFFFFFFF) /*Value high*/, "c"(0xC0000102) /*Set Kernel GS Base*/);
        asm volatile("wrmsr" ::"a"((uintptr_t)val & 0xFFFFFFFF) /*Value low*/,
                    "d"(((uintptr_t)val >> 32) & 0xFFFFFFFF) /*Value high*/, "c"(0xC0000101) /*Set Kernel GS Base*/);
    }

    inline cpu_t *getLocal()
    {
        cpu_t *ret;
        int intEnable = checkInterrupts();
        asm("cli");
        asm volatile("swapgs; movq %%gs:0, %0; swapgs;"
                    : "=r"(ret)); // CPU info is 16-byte aligned as per liballoc alignment
        if (intEnable)
            asm("sti");
        return ret;
    }
}
