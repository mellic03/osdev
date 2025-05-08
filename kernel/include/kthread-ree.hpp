#pragma once

#include <atomic>
#include <mutex>
#include <kdef.h>
#include <stdio.h>
#include <kernel/kstackframe.h>




enum kthread_: uint8_t
{
    kthread_Dead = 0,
    kthread_Ready,
    kthread_Sleeping,
    kthread_Running
};


// struct kthread_t
// {
//     kstackframe frame;

//     uint8_t     fxdata[512] __attribute__((aligned(16)));
//     uint8_t     stack[32*1024] __attribute__((aligned(16)));
//     uint8_t    *stack_top;

//     char     name[32];
//     size_t   tid;
//     uint8_t  status;
//     int64_t  timeLastAwake;
//     int64_t  sleepDuration;


//     kthread_t()
//     :   stack_top(stack + sizeof(stack) - 16)
//     {

//     };
// };


// namespace kthread
// {
//     extern bool isRunning;

//     uint8_t create( const char name[], void (*)(void*), void *arg, uint8_t *fxstate );

//     void start();
//     void sleep( uint64_t ms );
//     void yield();
//     void exit();    
//     uint8_t this_tid();
//     kthread_t *this_thread();
    
//     void start_handler( kstackframe* );
//     void schedule( kstackframe* );

// };



























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
// //     uint64_t isrno, errno;

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



