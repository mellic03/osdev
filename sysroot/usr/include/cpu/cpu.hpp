#pragma once
#include <stdint.h>
#include <cpuid.h>
#include <mutex>

#include "gdt.hpp"
#include "idt.hpp"
#include "scheduler.hpp"
// #include "kthread.hpp"

struct kthread_t;
struct ThreadScheduler;

struct cpu_t
{
    cpu_t *self;
    uint64_t id;

    ThreadScheduler *sched = nullptr;
    kthread_t  *idleThread = nullptr;
    kthread_t  *currThread = nullptr;

    uint64_t  syscall_no;
    uintptr_t syscall_req;
    uintptr_t syscall_res;

    void     *gdt;
    gdt_ptr_t gdtPtr;
    idt_ptr_t idtPtr;

    cpu_t(): self(nullptr), id(9999) {  };
    cpu_t( uint64_t lapic_id );

    kthread_t *createThread( const char *name, void (*fn)(void*), void *arg )
    { return this->sched->createThread(name, fn, arg); }

};

extern "C"
{
    // extern void cpu_fxsave( cpu_t* );
    // extern void cpu_fxrstor( cpu_t* );
    // extern uintptr_t GDT64_ptr;
    extern uint64_t cpu_readCR3();
    extern void cpu_writeCR3( uint64_t );
}


namespace SMP
{
    bool       is_initialized();

    cpu_t     *boot_cpu();

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

    void createIDT();
    void installIDT();
    void installISR( uint8_t isrno, isrHandlerFn fn );
    void installIRQ( uint8_t irqno, irqHandlerFn fn );

    void enableSSE();
    void fxsave( uint8_t *dst );
    void fxrstor( uint8_t *src );

    constexpr inline void cli() { asm volatile ("cli"); }
    constexpr inline void sti() { asm volatile ("sti"); }
    
    // static int cpuHasMSR()
    // {
    //     uint32_t eax, edx;
    //     __get_cpuid(1, &eax, nullptr, nullptr, &edx);
    //     return edx & CPUID_FLAG_MSR;
    // }

    uint64_t getCR3();
    void     setCR3( uint64_t );

    inline void getMSR(uint32_t msr, uint32_t *lo, uint32_t *hi)
    {
        __asm__ volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
    }
    
    inline void setMSR(uint32_t msr, uint32_t lo, uint32_t hi)
    {
        __asm__ volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
    }

    inline uint64_t getRFLAGS()
    {
        uint64_t rflags;
        asm volatile ( "pushfq\n\t"
                       "pop %0"
                       : "=r"(rflags)
                       : : "memory" );
        return rflags;
    }

    inline void setRFLAGS( uint64_t rflags )
    {
        asm volatile (
            "pushfq\n\t"
            "pop %0"
            : "=r"(rflags)
        );
    }

}


// inline void cpu_readMSR( uint32_t msr, uint32_t *lo, uint32_t *hi )
// {
//    asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
// }

// inline void cpu_writeMSR( uint32_t msr, uint32_t lo, uint32_t hi )
// {
//    asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
// }


// // inline static
// // void DoSwitch( CPU* cpu )
// // {
// //     asm volatile("fxrstor64 (%0)" ::"r"((uintptr_t)cpu->currentThread->fxState) : "memory");

// //     asm volatile("wrmsr" ::"a"(cpu->currentThread->fsBase & 0xFFFFFFFF) /*Value low*/,
// //                  "d"((cpu->currentThread->fsBase >> 32) & 0xFFFFFFFF) /*Value high*/, "c"(0xC0000100) /*Set FS Base*/);

// //     TSS::SetKernelStack(&cpu->tss, (uintptr_t)cpu->currentThread->kernelStack);

// //     cpu->currentThread->timeSlice = cpu->currentThread->timeSliceDefault;

// //     // Check for a few things
// //     // - Process is in usermode
// //     // - Pending unmasked signals
// //     // If true, invoke the signal handler
// //     if ((cpu->currentThread->registers.cs & 0x3) &&
// //         (cpu->currentThread->pendingSignals & ~cpu->currentThread->EffectiveSignalMask())) {
// //         if (cpu->currentThread->parent->State() == Process::Process_Running) {
// //             int ret = acquireTestLock(&cpu->currentThread->kernelLock);
// //             assert(!ret);

// //             cpu->currentThread->HandlePendingSignal(&cpu->currentThread->registers);
// //             releaseLock(&cpu->currentThread->kernelLock);
// //         }
// //     }

// //     asm volatile(
// //         R"(mov %0, %%rsp;
// //         mov %1, %%rax;
// //         pop %%r15;
// //         pop %%r14;
// //         pop %%r13;
// //         pop %%r12;
// //         pop %%r11;
// //         pop %%r10;
// //         pop %%r9;
// //         pop %%r8;
// //         pop %%rbp;
// //         pop %%rdi;
// //         pop %%rsi;
// //         pop %%rdx;
// //         pop %%rcx;
// //         pop %%rbx;
        
// //         mov %%rax, %%cr3
// //         pop %%rax
// //         addq $8, %%rsp
// //         iretq)" ::"r"(&cpu->currentThread->registers),
// //         "r"(cpu->currentThread->parent->GetPageMap()->pml4Phys));
// // }

