#include <cpu/scheduler.hpp>
#include <kthread.hpp>
#include "kernel/clock.hpp"
#include <cpu/cpu.hpp>
#include <kthread.hpp>
#include <kassert.h>
#include <kmalloc.h>
#include <string.h>



static void kthread_wrapper( void (*fn)(void*), void *arg )
{
    fn(arg);
    auto *th = SMP::this_thread();
    th->status = KThread_DEAD;

    while (true) { asm volatile ("hlt"); }
}




ThreadScheduler::ThreadScheduler( cpu_t *cpu )
:   m_cpu(cpu)
{
    kassert("No cpu", cpu != nullptr);
    memset(m_threads.data(), 0, m_threads.size() * sizeof(kthread_t*));
}




void
ThreadScheduler::start()
{
    m_currIdx = 0;
    KInterrupt<Int_KTHREAD_START>();
}




kthread_t*
ThreadScheduler::createThread( const char *name, void (*fn)(void*), void *arg )
{
    auto *th = (kthread_t*)kmalloc(sizeof(kthread_t));
    m_threads.push_back(th);

    memset(th->stack, 0, sizeof(th->stack));
    memset(th->name, '\0', sizeof(th->name));
    strncpy(th->name, name, sizeof(th->name));

    th->cpu              = m_cpu;
    th->tid              = m_threads.size() - 1;
    th->status           = KThread_READY;
    th->wakeTime         = 0;
    th->stackTop         = th->stack + sizeof(th->stack) - 16;

    th->frame.iret_rsp   = (uint64_t)(th->stackTop);
    th->frame.iret_rip   = (uint64_t)kthread_wrapper;
    th->frame.rdi        = (uint64_t)fn;
    th->frame.rsi        = (uint64_t)arg;
    th->frame.rbp        = 0;
    th->frame.iret_flags = 0x202; // CPU::getRFLAGS() ??
    // asm volatile("fxsave %0 " : : "m"(th->fxdata));

    return th;
}



uint8_t
ThreadScheduler::getNextIdx()
{
    uint8_t nextIdx = (m_currIdx+1) % m_threads.size();
    auto   *next    = m_threads[nextIdx];

    if (next->status != KThread_SLEEPING)
    {
        return nextIdx;
    }

    while (nextIdx != m_currIdx)
    {
        next = m_threads[nextIdx];

        if (kclock::now() >= next->wakeTime)
        {
            next->status = KThread_READY;
            return nextIdx;
        }

        nextIdx = (nextIdx+1) % m_threads.size();
    }

    return m_currIdx;
}




void swap_threads( kthread_t *curr, kthread_t *next, intframe_t *frame )
{
    uint64_t tmp_cs = frame->iret_cs;
    uint64_t tmp_ss = frame->iret_ss;

    // asm volatile("fxsave %0 " : : "m"(curr->fxdata));

    curr->status = KThread_READY;
    next->status = KThread_RUNNING;

    curr->frame = *frame;
    *frame = next->frame;

    frame->iret_cs = tmp_cs;
    frame->iret_ss = tmp_ss;

    // asm volatile("fxrstor %0 " : : "m"(next->fxdata));
}




void
ThreadScheduler::trampoline( intframe_t *frame )
{
    uint64_t frame_cs = frame->iret_cs;
    uint64_t frame_ss = frame->iret_ss;

    auto *curr = m_threads[m_currIdx];
    // asm volatile("fxsave %0 " : : "m"(curr->fxdata));

    *frame = curr->frame;
    frame->iret_cs = frame_cs;
    frame->iret_ss = frame_ss;

    m_cpu->currThread = curr;
}


void
ThreadScheduler::schedule( intframe_t *frame )
{
    if (m_cpu->currThread == nullptr)
    {
        return;
    }

    // for (size_t i=0; i<m_threads.size(); i++)
    // {
    //     if (m_threads[i]->status == KThread_DEAD)
    //     {
    //         removeThread(i);
    //         break;
    //     }
    // }

    uint8_t nextIdx = getNextIdx();
    auto *curr = m_threads[m_currIdx];
    auto *next = m_threads[nextIdx];

    // kassert("dead thread", curr->status != KThread_DEAD);
    // kassert("dead thread", next->status != KThread_DEAD);

    swap_threads(curr, next, frame);

    m_currIdx = nextIdx;
    m_cpu->currThread = m_threads[m_currIdx];
}







void ThreadScheduler::trampolineISR( intframe_t *frame )
{
    SMP::this_sched()->trampoline(frame);
}


#include <kernel/log.hpp>
void ThreadScheduler::scheduleISR( intframe_t *frame )
{
    SMP::this_sched()->schedule(frame);
}

