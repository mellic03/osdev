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
//         // syslog::printf("[threadManager_UpdateDead] \"%s\"\n", th->name);
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
//         // syslog::printf("[CPU%u threadManager]\n", cpu->id);
        
//         auto *sd = SMP::this_sched();
//         threadManager_UpdateDead(sd);
//         threadManager_UpdateSleeping(sd);
//         kthread::yield();
//     }
// }


void kthread::cullmain( void* )
{
    while (true)
    {
        asm volatile ("hlt");
    }
}

void kthread::idlemain( void* )
{
    while (true)
    {
        asm volatile ("hlt");
    }
}



static void kthread_wrapper( void (*threadmain)(void*), void *arg )
{
    threadmain(arg);
    kthread::yield(YldRsn_Exit);
    while (true) { asm volatile ("hlt"); }
}



ThreadScheduler::ThreadScheduler( cpu_t &cpu )
:   m_cpu(cpu), m_isRunning{false}, m_lock{false}
{
    m_isRunning.store(false);
    m_threads.clear();
    // kassert("No cpu", cpu != nullptr);
}



void
ThreadScheduler::start()
{
    // kassert(m_threads.size() > 0);
    addThread("idlemain", kthread::idlemain, nullptr);
    KInterrupt<IntNo_KTHREAD_START>();
}




kthread_t*
ThreadScheduler::allocateThread( const char *name, void (*fn)(void*), void *arg )
{
    // std::lock_guard lock(m_lock);
    // global_sched_lock.lock();
    auto *th = (kthread_t*)kmalloc(sizeof(kthread_t));

    memset(th->stack, 0, sizeof(th->stack));
    memset(&(th->frame), 0, sizeof(intframe_t));
    memset(th->name, '\0', sizeof(th->name));
    strncpy(th->name, name, sizeof(th->name));

    th->cpu              = &m_cpu;
    th->tid              = m_thread_tid++;
    th->status           = KThread_READY;
    th->wakeTime         = 0;
    th->stackTop         = th->stack + sizeof(th->stack) - 16;

    th->frame.rsp    = (uint64_t)(th->stackTop);
    th->frame.rip    = (uint64_t)kthread_wrapper;
    th->frame.rdi    = (uint64_t)fn;
    th->frame.rsi    = (uint64_t)arg;
    th->frame.rbp    = 0;
    th->frame.rflags = CPU::getRFLAGS(); // 0x202;
    // CPU::fxsave(th->fxstate);

    return th;
}



kthread_t*
ThreadScheduler::addThread( const char *name, void (*fn)(void*), void *arg )
{
    kthread_t *th = allocateThread(name, fn, arg);
    m_threads.push_back(th);
    return th;
}


void
ThreadScheduler::releaseThread( kthread_t *th )
{
    kfree(th);
}



/**
 * @return Next thread
 */
kthread_t *swap_threads( kthread_t *curr, kthread_t *next, intframe_t *frame )
{
    uint64_t tmp_cs = frame->cs;
    uint64_t tmp_ss = frame->ss;

    // asm volatile("fxsave %0 " : : "m"(curr->fxstate));

    curr->status = KThread_READY;
    next->status = KThread_RUNNING;

    curr->frame = *frame;
    *frame = next->frame;

    frame->cs = tmp_cs;
    frame->ss = tmp_ss;

    // asm volatile("fxrstor %0 " : : "m"(next->fxstate));

    return next;
}



void
ThreadScheduler::trampoline( intframe_t *frame )
{
    uint64_t frame_cs = frame->cs;
    uint64_t frame_ss = frame->ss;

    if (m_threads.empty())
    {
        addThread("idlemain", kthread::idlemain, nullptr);
    }

    auto *curr = m_threads.front();
    // CPU::fxsave(curr->fxstate);
    // asm volatile("fxsave %0 " : : "m"(curr->fxstate));

    *frame = curr->frame;
    frame->cs = frame_cs;
    frame->ss = frame_ss;

    m_isRunning.store(true);
}


void
ThreadScheduler::schedule( intframe_t *frame )
{
    if (m_isRunning.load() == false)
        return;
    kassert(m_threads.size() > 0);
        
    kthread_t *prev = m_threads.pop_front(); m_threads.push_back(prev);
    kthread_t *next = m_threads.front();

    // while (next != prev)
    // {
    //     if (next->status != KThread_SLEEPING)
    //         break;
    //     next = m_threads.pop_front(); m_threads.push_back(next);
    // }

    // CPU::fxsave(prev->fxstate);

    swap_threads(prev, next, frame);
    // CPU::fxrstor(next->fxstate);
}







void ThreadScheduler::trampolineISR( intframe_t *frame )
{
    // syslog::println("[CPU%lu trampolineISR]", SMP::this_cpu()->id);
    SMP::this_sched()->trampoline(frame);
}


#include <kernel/log.hpp>
void ThreadScheduler::scheduleISR( intframe_t *frame )
{
    // syslog::println("[CPU%lu scheduleISR]", SMP::this_cpu()->id);
    SMP::this_sched()->schedule(frame);
}

