#pragma once
#include "../smp/round_buffer.hpp"
#include <kernel/interrupt.hpp>
#include <kernel/lock.hpp>
#include <vector>
#include <algorithm>
#include <mutex>

struct cpu_t;
struct kthread_t;


class ThreadScheduler
{
private:
    cpu_t  &m_cpu;
    size_t  m_thread_tid = 0;

    void trampoline( intframe_t* );
    void schedule( intframe_t* );
    void check_sleepers();

public:
    knl::atomic_flag m_startLock;
    knl::atomic_flag m_switchLock;

    idk::RoundBuffer<kthread_t*, 64> m_threads;
    idk::RoundBuffer<kthread_t*, 64> m_sleeping;
    kthread_t *m_idlethread;
    // idk::static_vector<kthread_t*, 64> m_active;
    // idk::static_vector<kthread_t*, 64> m_dead;

    // static void trampolineISR( intframe_t* );
    static void scheduleISR( intframe_t* );

    ThreadScheduler( cpu_t& );

    kthread_t *currThread() { return m_threads.front(); }
    kthread_t *addThread( const char *name, void (*fn)(void*), void *arg );
    void       releaseThread( kthread_t *thread );
};

using ksched_t = ThreadScheduler;




// static void thread_wrapper( void (*fn)(void*), void *arg )
// {
//     fn(arg);
//     allThreads[currThread]->status = kthread_Dead;
//     printf("[kthread::wrapper] finished\n");
//     while (true) { asm volatile ("hlt"); }
// }


