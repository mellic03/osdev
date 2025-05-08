#pragma once
#include <static_vector.hpp>
#include <kernel/interrupt.hpp>

struct cpu_t;
struct kthread_t;


class ThreadScheduler
{
private:
    cpu_t  *m_cpu;
    uint8_t m_currIdx;

    uint8_t getNextIdx();
    void trampoline( intframe_t* );
    void schedule( intframe_t* );

public:
    idk::static_vector<kthread_t*, 32> m_threads;

    static void trampolineISR( intframe_t* );
    static void scheduleISR( intframe_t* );

    ThreadScheduler( cpu_t* );

    void       start();
    kthread_t *createThread( const char *name, void (*fn)(void*), void *arg );

};

using ksched_t = ThreadScheduler;




// static void thread_wrapper( void (*fn)(void*), void *arg )
// {
//     fn(arg);
//     allThreads[currThread]->status = kthread_Dead;
//     printf("[kthread::wrapper] finished\n");
//     while (true) { asm volatile ("hlt"); }
// }


