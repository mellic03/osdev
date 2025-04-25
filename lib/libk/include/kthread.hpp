#pragma once

#include <atomic>
#include <mutex>
#include <kdef.h>
#include <stdio.h>
#include <kstackframe.h>


struct kPCB;
struct kTCB;
class  kthread;


// namespace kernel
// {
//     kthread *this_thread();
// }



struct kTCB
{
    uint64_t tid;
    uint64_t rsp;
    uint64_t rip;
    uint64_t status;

    struct {
        uint64_t r11, r12, r13, r14, r15;
        uint64_t rax, rbx, rcx, rdx;
        uint64_t rdi, rsi, rbp;
    } registers;

    kTCB *next;

} __attribute__((packed));




enum kproc_status: uint8_t
{
    KPROC_DEAD = 0,
    KPROC_READY,
    KPROC_SLEEPING,
    KPROC_RUNNING
};




class kthread
{
private:
    static void idlemain(void*);
    static void ted_bundy( void* );
    static void add( kthread* );
    static void remove( kthread* );

public:
    static bool     running;
    static kthread *m_curr;
    
    char    name[32];
    size_t  tid;
    uint8_t status;
    uint8_t *stack;
    kstackframe frame;
    kthread *next;
    uint64_t timeToWake;
    uint64_t timer;

    kthread( const char thread_name[], void (*)(void*), void *arg );
    ~kthread();

    static void start();
    static void sleep( uint64_t ms );
    static void yield();
    static void exit();    
    static size_t this_tid();
    
    static void start_handler( kstackframe* );
    static void schedule( kstackframe* );
    static void wrapper( void (*)(void*), void* );

    
public:
    static std::mutex global_lock;
    inline static std::atomic_int lock_count = 0;


    class yield_guard
    {
    private:
        std::atomic_int &ref;
    
    public:
        yield_guard( std::atomic_int &value )
        : ref(value) { ref++; }
    
        ~yield_guard() { ref--; }
    
        yield_guard (const yield_guard& ) = delete;
        yield_guard &operator=( const yield_guard& ) = delete;
    
    };
    
    
};



























// // class ksched_refc
// // {
// // private:
// //     std::atomic_int &m_ct;

// // public:
// //     ksched_refc( std::atomic_int &ct )
// //      : m_ct(ct) { m_ct++; };

// //     ~ksched_refc() { m_ct--; };

// //     void wait()
// //     {
// //         while (m_ct.load() != 0)
// //         {
// //             kthread::sleep(10);
// //         }
// //     }

// //     bool try_wait()
// //     {
// //         return (m_ct.load() == 0);
// //     }
// // };

// struct kthread_frame
// {
//     uint64_t rbp, rsi, rdi, rdx, rcx, rbx, rax, r15, r14, r13, r12, r11, rsp, rip;
// };
// // struct kthread_frame
// // {
// //     uint64_t rip, rsp, r11, r12, r13, r14, r15, rax, rbx, rcx, rdx, rdi, rsi, rbp;
// // };

// // struct kthread_frame
// // {
// //     uint64_t r11, r12, r13, r14, r15;
// //     uint64_t rax, rbx, rcx, rdx;
// //     uint64_t rdi, rsi, rbp;
// //     uint64_t vcode, ecode;

// //     uint64_t ret_rip;
// //     uint64_t ret_cs;
// //     uint64_t ret_flags;
// //     uint64_t ret_rsp;
// //     uint64_t ret_ss;
// // };

// extern "C"
// {
//     // extern void kthread_start( void );
//     // extern void kthread_yield( void );
// }


// class kthread
// {
// private:
//     static void idlemain(void*);
//     static void ted_bundy( void* );
//     static void add( kthread* );
//     static void remove( kthread* );
//     static void wrapper( void (*)(void*), void* );

// public:
//     static std::atomic_int lock_count;
//     static bool     running;
//     static kthread *m_curr;
//     size_t  tid;
//     uint8_t status;
//     uint8_t *stack;
//     uint64_t rip, rsp, rdi, rsi, rbp, flags;
//     uint64_t rax, rbx, rcx, rdx;
//     kthread *next;
//     uint64_t lastTimeAwake;

//     kthread( void (*)(void*), void *arg );
//     ~kthread();

//     static void start();
//     static void sleep( uint64_t ms );
//     static void yield();
//     static void exit();    

//     static void start_handler( kstackframe* );
//     static void schedule( kstackframe* );
//     static size_t this_tid();

    
// public:

// };



