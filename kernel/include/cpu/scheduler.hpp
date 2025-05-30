#pragma once
#include <kernel/ringbuffer.hpp>
#include <sys/interrupt.hpp>
#include <smp/lock.hpp>
#include <vector>
#include <algorithm>
#include <mutex>

struct cpu_t;
struct kthread_t;


class ThreadScheduler
{
private:
    size_t m_thread_tid = 0;

    void trampoline( intframe_t* );
    void schedule( intframe_t* );
    void check_sleepers();

public:
    knl::AtomicFlag m_startLock;
    std::atomic_int m_switchCount{0};

    knl::RingBuffer<kthread_t*, 64> m_threads;
    knl::RingBuffer<kthread_t*, 64> m_sleeping;
    kthread_t *m_idlethread;
    // idk::static_vector<kthread_t*, 64> m_active;
    // idk::static_vector<kthread_t*, 64> m_dead;

    // static void trampolineISR( intframe_t* );
    static void scheduleISR( intframe_t* );

    ThreadScheduler();

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


