#include <sys/process.hpp>
#include <cpu/cpu.hpp>

#include <kernel/bitmanip.hpp>
#include <kernel/clock.hpp>
#include <kernel/log.hpp>
#include <kmemxx.hpp>
#include <kmalloc.h>
#include <kassert.h>
#include <string.h>
#include <new>


static void thread_wrapper( void (*tmain)(void*), void *arg )
{
    asm volatile ("sti");
    tmain(arg);
    // kthread::yield();
    while (true) { CPU::hlt(); }
}

static void idlemain( void* )
{
    while (true)
    {
        asm volatile ("hlt");
    }
}


knl::Sched::Sched()
{
    m_startLock.set();
    m_switchCount.store(0);
    m_threads.clear();
    m_sleeping.clear();

    m_idlethread = createThread(idlemain, nullptr);
}


/*
    https://github.com/LemonOSProject/LemonOS/blob/15f607e8be86fcaf9c86a68c91915e820c9e935e/Kernel/src/Arch/x86_64/Thread.cpp#L48
*/

#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>


knl::Thread*
knl::Sched::createThread( void (*tmain)(void*), void *arg )
{
    auto *th = (Thread*)kmalloc(sizeof(Thread));
          th = new (th) Thread();
    m_threads.push_back(th);

    th->process  = nullptr;
    th->status   = Thread_Ready;
    th->wakeTime = 0;

    #if defined(__SSE__) && defined(__AVX__)
        kmemset<uint8_t>(th->fxstate, 0, sizeof(th->fxstate));
        uint32_t mask = 0x1000|0x0800|0x0400|0x0200|0x0100|0x0080;
        ((CPU::fxstate_t*)th->fxstate)->mxcsr     = 0x1F80;
        ((CPU::fxstate_t*)th->fxstate)->mxcsrMask = mask;
        ((CPU::fxstate_t*)th->fxstate)->fcw       = 0x033F;
    #endif

    kmemset<uint8_t>(th->stack, 0, sizeof(th->stack));
    th->stackTop = idk::align_down(th->stack + sizeof(th->stack), 16);

    kmemset<uint8_t>(&(th->frame), 0, sizeof(intframe_t));
    th->frame.rsp    = (uint64_t)(th->stackTop);
    th->frame.rip    = (uint64_t)thread_wrapper;
    th->frame.rdi    = (uint64_t)tmain;
    th->frame.rsi    = (uint64_t)arg;
    th->frame.rbp    = 0;
    th->frame.rflags = CPU::getRFLAGS(); // 0x202;
    th->frame.cs     = CPU::GDT_OFFSET_KERNEL_CODE;
    th->frame.ss     = CPU::GDT_OFFSET_KERNEL_DATA;

    return th;
}




void
knl::Sched::releaseThread( Thread *th )
{
    // VMM::free(th, sizeof(Thread));
    kfree(th);
}




void
knl::Sched::trampoline( intframe_t *frame )
{
    uint64_t frame_cs = frame->cs;
    uint64_t frame_ss = frame->ss;

    auto *curr = m_threads.front();

    *frame = curr->frame;
    frame->cs = frame_cs;
    frame->ss = frame_ss;

    #if defined(__SSE__) && defined(__AVX__)
        CPU::fxrstor(curr->fxstate);
    #endif

    if (curr->process)
    {
        CPU::setCR3(curr->process->m_pml4);
    }

    m_startLock.clear();
}



/**
 * @return Next thread
 */
knl::Thread *swap_threads( knl::Thread *curr, knl::Thread *next, intframe_t *frame )
{
    uint64_t tmp_cs = frame->cs;
    uint64_t tmp_ss = frame->ss;

    curr->status = knl::Thread_Ready;
    next->status = knl::Thread_Running;

    curr->frame = *frame;
    *frame = next->frame;

    frame->cs = tmp_cs;
    frame->ss = tmp_ss;

    return next;
}



void
knl::Sched::check_sleepers()
{
    size_t size = m_sleeping.size();

    for (size_t i=0; i<size; i++)
    {
        if (m_sleeping.empty())
            return;

        Thread *curr = m_sleeping.front();
        if (kclock::now() >= curr->wakeTime)
        {
            curr->status = knl::Thread_Ready;
            curr = m_sleeping.pop_front();
            m_threads.push_back(curr);
        }

        else
        {
            m_sleeping.rotate(1);
        }
    }
}


void
knl::Sched::schedule( intframe_t *frame )
{
    if (m_switchCount++ > 0)
    {
        m_switchCount--;
        return;
    }

    Thread *prev = nullptr;
    Thread *next = nullptr;
    prev = m_threads.front();

    if (prev->status != Thread_Sleeping)
    {
        m_threads.rotate(1);
        next = m_threads.front();
    }

    else
    {
        prev = m_threads.pop_front();
        m_sleeping.push_back(prev);
        next = m_threads.front();
    }

    if (next == m_idlethread)
    {
        m_threads.rotate(1);
        next = m_threads.front();
    }

    check_sleepers();

    // syslog::println(
    //     "[ThreadScheduler::schedule CPU%lu] %s --> %s",
    //     SMP::this_cpuid(), prev->name, next->name
    // );


    swap_threads(prev, next, frame);

    // if (prev->useSSE)
    // if (next->useSSE)
    #if defined(__SSE__) && defined(__AVX__)
        CPU::fxsave(prev->fxstate);
        CPU::fxrstor(next->fxstate);
    #endif

    if (next->process && (next->process != prev->process))
    {
        CPU::setCR3(next->process->m_pml4);
    }

    m_switchCount--;
}







// void ThreadScheduler::trampolineISR( intframe_t *frame )
// {
//     syslog::println("[trampolineISR] cpu=0x%lx", SMP::this_cpu());
//     // syslog::println("[CPU%lu trampolineISR]", SMP::this_cpu()->id);
//     SMP::this_sched()->trampoline(frame);
// }


void knl::Sched::scheduleISR( intframe_t *frame )
{
    // syslog::println("[CPU%lu scheduleISR]", SMP::this_cpu()->id);

    auto &sd = SMP::this_cpu()->sched2;
    if (sd.m_startLock.isset())
        sd.trampoline(frame);
    else
        sd.schedule(frame);
}

