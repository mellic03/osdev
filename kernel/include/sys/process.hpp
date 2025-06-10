#pragma once

#include <stddef.h>
#include <stdint.h>
#include <sys/interrupt.hpp>
#include <kernel/linkedlist.hpp>
#include <kernel/ringbuffer.hpp>
#include <smp/lock.hpp>


struct cpu_t;

namespace knl
{
    static constexpr size_t MAX_PROCESSES = 64;

    struct Process;
    struct Thread;
    struct Sched;

    enum Thread_: uint32_t
    {
        Thread_Ready,
        Thread_Running,
        Thread_Sleeping,
        Thread_Dead
    };

    extern knl::LinkedList<Process> AllProcs;
    extern knl::Process *InitProc;

    Process *createProcess( const char *name, void (*pmain)(void*), void *arg );
    Thread  *createThread( Process *P, void (*tmain)(void*), void *arg );
    void     threadYield();
    
}



struct knl::Process: public knl::LinkedListNode
{
public:
    char      m_name[64];
    uint64_t  m_pid;
    uintptr_t m_pml4; // physical address
    knl::LinkedList<Thread> m_threads;

    Process() {  };
    Process( const char *name );
};



struct knl::Thread: public knl::LinkedListNode
{
public:
    static constexpr size_t STACK_SIZE = 16 * 1024;

    Process   *process;
    Thread_    status;
    uint64_t   wakeTime;

    intframe_t frame;
    uint8_t   *stackTop;
    uint8_t    stack[STACK_SIZE] __attribute__((aligned(16)));

    #if defined(__SSE__) && defined(__AVX__)
        uint8_t fxstate[4096] __attribute__((aligned(16)));
    #endif

    Thread(): LinkedListNode() {  };

};



class knl::Sched
{
private:
    size_t m_thread_tid = 0;

    void trampoline( intframe_t* );
    void schedule( intframe_t* );
    void check_sleepers();

public:
    knl::AtomicFlag m_startLock;
    std::atomic_int m_switchCount{0};

    knl::RingBuffer<Thread*, 64> m_threads;
    knl::RingBuffer<Thread*, 64> m_sleeping;
    Thread *m_idlethread;

    static void scheduleISR( intframe_t* );

    Sched();

    Thread *currThread()
    {
        return (m_threads.empty()) ? nullptr : m_threads.front();
    }

    Thread *createThread( void (*fn)(void*), void *arg );
    void    releaseThread( Thread *thread );
};

