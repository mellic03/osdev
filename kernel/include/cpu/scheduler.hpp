#pragma once
#include "../smp/round_buffer.hpp"
#include <kernel/interrupt.hpp>
#include <vector>
#include <algorithm>
#include <mutex>

struct cpu_t;
struct kthread_t;


template <typename T>
void vec_swap_pop( T &vec, size_t idx )
{
    std::swap(vec[idx], vec.back());
    vec.pop_back();
}

template <typename T, typename U>
void vec_insert( T &vec, const U &data )
{
    for (size_t i=0; i<vec.size(); i++)
        if (vec[i] == data)
            return;
    vec.push_back(data);
}

template <typename T, typename U>
void vec_remove( T &vec, const U &data )
{
    for (size_t i=0; i<vec.size(); i++)
    {
        if (vec[i] == data)
        {
            vec_swap_pop(vec, i);
            return;
        }
    }
}


namespace kthread
{
    void cullmain( void* );
    void idlemain( void* );
}

class ThreadScheduler
{
private:
    cpu_t  &m_cpu;
    size_t  m_thread_tid = 0;

    void trampoline( intframe_t* );
    void schedule( intframe_t* );

public:
    std::atomic_bool m_isRunning{false};
    std::mutex       m_lock;

    // std::vector<kthread_t*> m_threads;
    idk::RoundBuffer<kthread_t*, 64> m_threads;
    // idk::static_vector<kthread_t*, 64> m_active;
    // idk::static_vector<kthread_t*, 64> m_sleeping;
    // idk::static_vector<kthread_t*, 64> m_dead;

    static void trampolineISR( intframe_t* );
    static void scheduleISR( intframe_t* );

    ThreadScheduler( cpu_t& );

    void       start();
    kthread_t *allocateThread( const char *name, void (*fn)(void*), void *arg );
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


