// #pragma once

// #include <kdef.h>
// #include <kstackframe.h>




// struct kproc_t
// {
//     size_t tid;
//     uint64_t rip, rsp, rdi, rbp;
//     kproc_t *next;
// };


// void kproc_switch( kstackframe* );
// void kproc_schedule( kstackframe* );

// void kproc_init();
// void kproc_yield();
// kproc_t *kproc_new( void (*)(void*), void *arg );
// void kproc_detach( kproc_t* );







// struct klock_t
// {
//     bool locked;
// };

// void klock_acquire( klock_t* );
// void klock_release( klock_t* );



// // enum KProcStatus: uint32_t
// // {
// //     PROC_READY,
// //     PROC_RUNNING,
// //     PROC_SLEEPING,
// //     PROC_DEAD
// // };


// // struct KProcess
// // {
// //     size_t      pid;
// //     int32_t     sleep;
// //     uint8_t     priority;
// //     KProcStatus status;
// //     KProcess   *next;
// //     // uint64_t    stack[4096];
// //     uint64_t    rip, rsp, rdi, rbp;
    
// //     char name[64];

// //     KProcess( size_t id, const char*, void (*)(void*), void* );
// // };



// // class Scheduler
// // {
// // private:
// //     KProcess *m_list;
// //     KProcess *m_curr;
// //     size_t    m_pid;

// //     void add_process( KProcess *proc );

// // public:
// //     Scheduler();
// //     void schedule( kstackframe* );
// //     void sleep( size_t pid );
// //     KProcess *createProcess( const char *name, void(*function)(void*), void *arg );

// // };


