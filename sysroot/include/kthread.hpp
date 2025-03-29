#pragma once

#include <kdef.h>
#include <kstackframe.h>



struct klock_t
{
    bool locked;
};

void klock_acquire( klock_t* );
void klock_release( klock_t* );




enum kproc_status: uint8_t
{
    KPROC_DEAD = 0,
    KPROC_READY,
    KPROC_RUNNING
};



class kthread
{
private:
    static void add( kthread* );
    static void remove( kthread* );
    static void ted_bundy( void* );
    static void wrapper( void (*)(void*), void* );
    inline static bool     m_global_lock = false;
    inline static kthread *m_curr  = nullptr;
    inline static bool     m_first = true;

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

    static void   global_lock();
    static void   global_unlock();
    static size_t this_tid();
    static void   yield();
    static void   exit();
    
};



