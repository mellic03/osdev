// #pragma once

// #include <kernel/ringbuffer.hpp>
// #include <sys/interrupt.hpp>
// #include <smp/lock.hpp>
// #include <vector>
// #include <algorithm>
// #include <mutex>

// struct cpu_t;

// namespace SMP
// {
//     struct process_t;
//     struct thread_t;
//     struct sched_t;
// }



// class knl::sched_t
// {
// private:
//     size_t m_thread_tid = 0;

//     void trampoline( intframe_t* );
//     void schedule( intframe_t* );
//     void check_sleepers();

// public:
//     knl::AtomicFlag m_startLock;
//     std::atomic_int m_switchCount{0};

//     knl::RingBuffer<thread_t*, 64> m_threads;
//     knl::RingBuffer<thread_t*, 64> m_sleeping;
//     thread_t *m_idlethread;

//     static void scheduleISR( intframe_t* );

//     sched_t();

//     thread_t *currThread()
//     {
//         return (m_threads.empty()) ? nullptr : m_threads.front();
//     }

//     thread_t *createThread( process_t *P, void (*fn)(void*), void *arg );
//     void      releaseThread( thread_t *thread );
// };


