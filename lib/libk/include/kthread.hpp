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



// class ksched_refc
// {
// private:
//     std::atomic_int &m_ct;

// public:
//     ksched_refc( std::atomic_int &ct )
//      : m_ct(ct) { m_ct++; };

//     ~ksched_refc() { m_ct--; };

//     void wait()
//     {
//         while (m_ct.load() != 0)
//         {
//             kthread::sleep(10);
//         }
//     }

//     bool try_wait()
//     {
//         return (m_ct.load() == 0);
//     }
// };

struct kthread_frame
{
    uint64_t rip, rsp, r11, r12, r13, r14, r15, rax, rbx, rcx, rdx, rdi, rsi, rbp;
};



class kthread
{
private:

    static void idlemain(void*);
    static void ted_bundy( void* );
    static void add( kthread* );
    static void remove( kthread* );
    static void wrapper( void (*)(void*), void* );

public:
    static bool     running;
    static kthread *m_curr;
    size_t  tid;
    uint8_t status;
    uint8_t *stack;
    uint64_t rip, rsp, rdi, rsi, rbp, flags;
    uint64_t rax, rbx, rcx, rdx;
    kthread *next;
    uint64_t lastTimeAwake;

    kthread( void (*)(void*), void *arg );
    ~kthread();

    static void start();
    static void sleep( uint64_t ms );
    static void yield();
    static void exit();    

    static void start_handler( kstackframe* );
    static void schedule( kstackframe* );
    static size_t this_tid();

    
public:
    class refc
    {
    private:
        // static std::atomic_int m_ct;

    public:
         refc() {}; // { m_ct++; };
        ~refc() {}; // { m_ct--; };

        static bool try_wait() { return true; }
        // { return (m_ct.load() == 0); }
    };
};



