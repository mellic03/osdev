#pragma once

#include <atomic>
#include <kdef.h>
#include <kstackframe.h>


enum kproc_status: uint8_t
{
    KPROC_DEAD = 0,
    KPROC_READY,
    KPROC_RUNNING
};


class kthread
{
private:
    inline static bool global_lock = false;
    inline static kthread *m_curr  = nullptr;
    inline static bool     m_first = true;

    // static kthread *TedBundy;
    static void ted_bundy( void* );
    static void add( kthread* );
    static void remove( kthread* );
    static void wrapper( void (*)(void*), void* );

    size_t  tid;
    uint8_t status;
    uint8_t *stack;
    uint64_t rip, rsp, rdi, rsi, rbp, flags;
    uint64_t rax, rbx, rcx, rdx;
    kthread *next;

public:

    static void schedule( kstackframe* );
    
// public:
    kthread( void (*)(void*), void *arg );
    ~kthread();

    static size_t this_tid();
    static void   yield();
    static void   exit();    

public:
    struct lock_guard
    {
    private:
        // std::mutex  m_mutex;
        // std::lock_guard m_lock;

    public:
         lock_guard() { global_lock = true;  };
        ~lock_guard() { global_lock = false; };
    };

};



