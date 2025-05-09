#include <cpu/scheduler.hpp>
#include <kthread.hpp>
#include "kernel/clock.hpp"
#include "kernel/log.hpp"
#include <cpu/cpu.hpp>
#include <kthread.hpp>
#include <kassert.h>
#include <kmalloc.h>
#include <string.h>



static void threadManager_UpdateDead( ThreadScheduler *sd )
{
    auto &dead = sd->m_dead;

    while (dead.size() > 0)
    {
        auto *th = dead.back(); dead.pop_back();
        // syslog::printf("[threadManager_UpdateDead] \"%s\"\n", th->name);
        vec_remove(sd->m_threads, th);
        sd->releaseThread(th);
    }
}

static void threadManager_UpdateSleeping( ThreadScheduler *sd )
{
    // auto &active   = sd->m_active;
    auto &sleeping = sd->m_sleeping;

    for (int i=0; i<int(sleeping.size()); i++)
    {
        kthread_t *th = sleeping[i];
        if (kclock::now() >= th->wakeTime)
        {
            th->status = KThread_READY;
            vec_remove(sleeping, th);
            // vec_insert(sd.t, th);
            i -= 1;
        }
    }
}



static void threadManager( void* )
{
    while (true)
    {
        // auto *cpu = SMP::this_cpu();
        // syslog::printf("[CPU%u threadManager]\n", cpu->id);
        
        auto *sd = SMP::this_sched();
        threadManager_UpdateDead(sd);
        threadManager_UpdateSleeping(sd);
        kthread::yield();
    }
}










static void kthread_wrapper( void (*fn)(void*), void *arg )
{
    // syslog::printf("[kthread_wrapper] entry\n");
    fn(arg);
    // syslog::printf("[kthread_wrapper] exit\n");

    auto *th = SMP::this_thread();
    auto *sd = SMP::this_sched();
    sd->makeDead(th);
    kthread::yield();

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
    createThread("threadManager", threadManager, nullptr);
    m_currIdx = 0;
    KInterrupt<IntNo_KTHREAD_START>();
}




kthread_t*
ThreadScheduler::createThread( const char *name, void (*fn)(void*), void *arg )
{
    auto *th = (kthread_t*)kmalloc(sizeof(kthread_t));

    memset(th->stack, 0, sizeof(th->stack));
    memset(th->name, '\0', sizeof(th->name));
    strncpy(th->name, name, sizeof(th->name));

    th->cpu              = m_cpu;
    th->tid              = m_thread_tid++;
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

    m_threads.push_back(th);
    makeReady(th);

    return th;
}

void
ThreadScheduler::releaseThread( kthread_t *th )
{
    kfree(th);
}


void
ThreadScheduler::makeDead( kthread_t *th )
{
    CPU::cli();
    th->status = KThread_DEAD;
    vec_insert(m_dead, th);
    vec_remove(m_threads, th);
    CPU::sti();
}


void
ThreadScheduler::makeSleeping( kthread_t *th )
{
    CPU::cli();
    th->status = KThread_SLEEPING;
    vec_insert(m_sleeping, th);
    // vec_remove(m_threads, th);
    CPU::sti();
}

void
ThreadScheduler::makeReady( kthread_t *th )
{
    CPU::cli();
    th->status = KThread_READY;
    vec_insert(m_threads, th);
    vec_remove(m_sleeping, th);
    CPU::sti();
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
        return;
    if (m_threads.empty())
        return;

    uint8_t nextIdx = (m_currIdx+1) % m_threads.size();
    kthread_t *curr = m_threads[m_currIdx];
    kthread_t *next = m_threads[nextIdx];

    while (next != curr)
    {
        if (next->status != KThread_SLEEPING)
            break;
        nextIdx = (nextIdx+1) % m_threads.size();
        next    = m_threads[nextIdx];
    }

    m_currIdx = nextIdx;

    swap_threads(curr, next, frame);

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

