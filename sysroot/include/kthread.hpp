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
    uint64_t rbp, rsi, rdi, rdx, rcx, rbx, rax, r15, r14, r13, r12, r11, rsp, rip;
};
// struct kthread_frame
// {
//     uint64_t rip, rsp, r11, r12, r13, r14, r15, rax, rbx, rcx, rdx, rdi, rsi, rbp;
// };

// struct kthread_frame
// {
//     uint64_t r11, r12, r13, r14, r15;
//     uint64_t rax, rbx, rcx, rdx;
//     uint64_t rdi, rsi, rbp;
//     uint64_t vcode, ecode;

//     uint64_t ret_rip;
//     uint64_t ret_cs;
//     uint64_t ret_flags;
//     uint64_t ret_rsp;
//     uint64_t ret_ss;
// };

extern "C"
{
    extern void kthread_start( void );
    extern void kthread_yield( void );
}


class kthread
{
private:
    static void idlemain(void*);
    static void ted_bundy( void* );
    static void add( kthread* );
    static void remove( kthread* );
    static void wrapper( void (*)(void*), void* );

public:
    static std::atomic_int lock_count;
    static bool     running;
    static kthread *m_curr;
    size_t  tid;
    uint8_t status;
    uint8_t *stack;
    uint64_t rbp, rsi, rdi, rdx, rcx, rbx, rax;
    uint64_t r15, r14, r13, r12, r11, rsp, rip;
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
    // class refc
    class yield_lock
    {
    private:
        // static std::atomic_int m_ct;

    public:
         yield_lock() { kthread::lock_count++; };
        ~yield_lock() { kthread::lock_count--; };

        static bool try_wait()
        { return kthread::lock_count.load() == 0; }
    };
};



