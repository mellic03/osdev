#include <cpu/scheduler.hpp>
#include <kthread.hpp>
#include "kernel/clock.hpp"
#include "kernel/log.hpp"
#include <cpu/cpu.hpp>
#include <kthread.hpp>
#include <kassert.h>
#include <kmalloc.h>
#include <string.h>


// static std::mutex global_sched_lock;


// static void threadManager_UpdateDead( ThreadScheduler *sd )
// {
//     auto &dead = sd->m_dead;

//     while (dead.size() > 0)
//     {
//         auto *th = dead.back(); dead.pop_back();
//         // syslog::print("[threadManager_UpdateDead] \"%s\"\n", th->name);
//         vec_remove(sd->m_threads, th);
//         sd->releaseThread(th);
//     }
// }

// static void threadManager_UpdateSleeping( ThreadScheduler *sd )
// {
//     // auto &active   = sd->m_active;
//     auto &sleeping = sd->m_sleeping;

//     for (int i=0; i<int(sleeping.size()); i++)
//     {
//         kthread_t *th = sleeping[i];
//         if (kclock::now() >= th->wakeTime)
//         {
//             th->status = KThread_READY;
//             vec_remove(sleeping, th);
//             // vec_insert(sd.t, th);
//             i -= 1;
//         }
//     }
// }



// static void threadManager( void* )
// {
//     while (true)
//     {
//         // auto *cpu = SMP::this_cpu();
//         // syslog::print("[CPU%u threadManager]\n", cpu->id);
        
//         auto *sd = SMP::this_sched();
//         threadManager_UpdateDead(sd);
//         threadManager_UpdateSleeping(sd);
//         kthread::yield();
//     }
// }


// static void cullmain( void* )
// {
//     while (true)
//     {
//         asm volatile ("hlt");
//     }
// }


static void kthread_wrapper( void (*threadmain)(void*), void *arg )
{
    asm volatile ("sti");
    threadmain(arg);
    kthread::yield();
    while (true) { CPU::hlt(); }
}

static void idlemain( void* )
{
    while (true)
    {
        asm volatile ("hlt");
    }
}


ThreadScheduler::ThreadScheduler( cpu_t &cpu )
:   m_cpu(cpu)
{
    m_startLock.set();
    m_switchCount.store(0);
    m_threads.clear();
    m_sleeping.clear();

    m_idlethread = addThread("idlemain", idlemain, nullptr);
    // kassert("No cpu", cpu != nullptr);
}



kthread_t*
ThreadScheduler::addThread( const char *name, void (*fn)(void*), void *arg )
{
    auto *th = (kthread_t*)kmalloc(sizeof(kthread_t));
          th->setPriority(5);
    m_threads.push_back(th);

    memset(th->stack, 0, sizeof(th->stack));
    memset(&(th->frame), 0, sizeof(intframe_t));
    memset(th->name, '\0', sizeof(th->name));
    strncpy(th->name, name, sizeof(th->name));

    th->cpu          = &m_cpu;
    th->tid          = m_thread_tid++;
    th->status       = KThread_READY;
    th->wakeTime     = 0;
    th->stackTop     = th->stack + sizeof(th->stack) - 16;

    th->frame.rsp    = (uint64_t)(th->stackTop);
    th->frame.rip    = (uint64_t)kthread_wrapper;
    th->frame.rdi    = (uint64_t)fn;
    th->frame.rsi    = (uint64_t)arg;
    th->frame.rbp    = 0;
    th->frame.rflags = CPU::getRFLAGS(); // 0x202;
    CPU::fxsave(th->fxstate);
    // asm volatile("fxsave %0 " : : "m"(th->fxstate));

    return th;
}


void
ThreadScheduler::releaseThread( kthread_t *th )
{
    kfree(th);
}




void
ThreadScheduler::trampoline( intframe_t *frame )
{
    uint64_t frame_cs = frame->cs;
    uint64_t frame_ss = frame->ss;

    auto *curr = m_threads.front();
    CPU::fxsave(curr->fxstate);

    *frame = curr->frame;
    frame->cs = frame_cs;
    frame->ss = frame_ss;

    m_startLock.clear();
}



/**
 * @return Next thread
 */
kthread_t *swap_threads( kthread_t *curr, kthread_t *next, intframe_t *frame )
{
    uint64_t tmp_cs = frame->cs;
    uint64_t tmp_ss = frame->ss;

    curr->status = KThread_READY;
    next->status = KThread_RUNNING;

    curr->frame = *frame;
    *frame = next->frame;

    frame->cs = tmp_cs;
    frame->ss = tmp_ss;

    return next;
}



void
ThreadScheduler::check_sleepers()
{
    size_t size = m_sleeping.size();

    for (size_t i=0; i<size; i++)
    {
        if (m_sleeping.empty())
            return;

        kthread_t *curr = m_sleeping.front();
        if (kclock::now() >= curr->wakeTime)
        {
            curr->status = KThread_READY;
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
ThreadScheduler::schedule( intframe_t *frame )
{
    if (m_switchCount++ > 0)
    {
        m_switchCount--;
        return;
    }

    kthread_t *prev, *next;
    prev = m_threads.front();

    if (prev->status != KThread_SLEEPING)
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

    m_switchCount--;
}







// void ThreadScheduler::trampolineISR( intframe_t *frame )
// {
//     syslog::println("[trampolineISR] cpu=0x%lx", SMP::this_cpu());
//     // syslog::println("[CPU%lu trampolineISR]", SMP::this_cpu()->id);
//     SMP::this_sched()->trampoline(frame);
// }


#include <kernel/log.hpp>
void ThreadScheduler::scheduleISR( intframe_t *frame )
{
    // syslog::println("[scheduleISR] CPU=%lu", SMP::this_cpuid());

    auto *sd = SMP::this_sched();
    CPU::fxsave(SMP::this_thread()->fxstate);

    if (sd->m_startLock.isset())
        sd->trampoline(frame);
    else
        sd->schedule(frame);

    CPU::fxrstor(SMP::this_thread()->fxstate);

}

