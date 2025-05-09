#pragma once

#ifndef KTHREAD_H
    #define KTHREAD_H
#endif



#include "cpu/cpu.hpp"
#include <kernel/interrupt.hpp>

enum KThread_: uint32_t
{
    KThread_READY,
    KThread_RUNNING,
    KThread_SLEEPING,
    KThread_DEAD
};


struct kthread_t
{
    // static constexpr size_t fpreg_size = 512;
    static constexpr size_t stack_size = 16 * 1024;
    
    char       name[32];
    cpu_t     *cpu;
    uint32_t   tid;
    KThread_   status;
    uint64_t   wakeTime;

    intframe_t frame;
    uint8_t   *stackTop;
    uint8_t    stack[stack_size]  __attribute__((aligned(16)));
    // uint8_t    fxdata[fpreg_size] __attribute__((aligned(16)));
};


namespace kthread
{
    void yield();
    void sleep( uint64_t ms );
    void exit();
    
    kthread_t *create( const char *name, void (*fn)(void*), void *arg );
    void start();
    void idlemain( void* );

    // namespace detail
    // {
    //     void exit()
    //     {
    //         kthread::this_thread()->status = KThread_DEAD;
    //     }
    // }
}

// #define kthread_exit

