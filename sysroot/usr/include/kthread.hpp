#pragma once

#ifndef KTHREAD_H
    #define KTHREAD_H
#endif


#include <algorithm>
#include "cpu/cpu.hpp"
#include <sys/interrupt.hpp>

enum KThread_: uint32_t
{
    KThread_READY,
    KThread_RUNNING,
    KThread_SLEEPING,
    KThread_DEAD
};


struct kthread_t
{
private:
    uint8_t m_priority;

public:
    static constexpr size_t stack_size = 16 * 1024;
    static constexpr size_t fxsize = 2048;
    
    char       name[32];
    cpu_t     *cpu;
    uint32_t   tid;
    KThread_   status;
    uint64_t   wakeTime;

    intframe_t frame;
    uint8_t   *stackTop;
    uint8_t    stack[stack_size]  __attribute__((aligned(16)));
    uint8_t    fxstate[fxsize]    __attribute__((aligned(16)));

    void    setPriority( uint8_t p ) { m_priority = std::clamp(p, (uint8_t)1, (uint8_t)10); };
    uint8_t getPriority() { return m_priority; };

};


namespace kthread
{
    void yield();
    void sleep( uint64_t ms );
    void exit();
    
    kthread_t *create( const char *name, void (*fn)(void*), void *arg );
    // void start();

    // namespace detail
    // {
    //     void exit()
    //     {
    //         kthread::this_thread()->status = KThread_DEAD;
    //     }
    // }
}

// #define kthread_exit

