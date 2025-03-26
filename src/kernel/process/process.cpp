// #include "process.hpp"
// #include <kmalloc.h>
// #include <kintcode.h>
// #include <kinterrupt.h>
// #include <stdio.h>
// #include <string.h>
// #include "../log/log.hpp"


// kproc_t *m_curr = nullptr;


// kproc_t *__kproc_switch( kproc_t *thread, kstackframe *frame )
// {
//     frame->iret_rsp = thread->rsp;
//     frame->iret_rip = thread->rip;
//     frame->rdi      = thread->rdi;
//     frame->rbp      = thread->rbp;
//     return thread;
// }


// void kproc_switch( kstackframe *frame )
// {
//     if (m_curr && m_curr->next)
//     {
//         m_curr = __kproc_switch(m_curr->next, frame);
//     }
// }


// void kproc_schedule( kstackframe *frame )
// {
//     if (m_curr == nullptr)
//     {
//         return;
//     }

//     if (m_curr->next != nullptr)
//     {
//         m_curr = __kproc_switch(m_curr->next, frame);
//     }
// }







// static void __kproc_add( kproc_t *th )
// {
//     if (m_curr == nullptr)
//     {
//         m_curr = th;
//         m_curr->next = m_curr;
//     }

//     else if (m_curr == m_curr->next)
//     {
//         m_curr->next = th;
//         th->next = m_curr;
//     }

//     else
//     {
//         th->next = m_curr->next;
//         m_curr->next = th;
//     }
// }


// void kproc_init()
// {
//     m_curr = nullptr;
//     // threads.init(new KThread*[16], 16);
// }

// void kproc_yield()
// {
//     KInterrupt<INT_PROCESS_SWITCH>();
// }



// kproc_t *kproc_new( void (*fn)(void*), void *arg )
// {
//     uint8_t *stack = (uint8_t*)kmalloc(4096);
//     uint8_t *top   = stack + 4096;
//              top   = (uint8_t *)((uintptr_t)top & ~0xF);
//              top -= sizeof(void *);
//     memset(stack, 0, 4096);


//     kproc_t *thread = (kproc_t*)kmalloc(sizeof(kproc_t));

//     thread->rip = (uint64_t)fn;
//     thread->rsp = (uint64_t)top;
//     thread->rdi = (uint64_t)arg;
//     thread->rbp = 0;
//     thread->next = nullptr;

//     return thread;
// }


// void kproc_detach( kproc_t *thread )
// {
//     __kproc_add(thread);
// }








// void klock_acquire( klock_t *lock )
// {
//     while (true)
//     {
//         if (lock->locked == false)
//         {
//             lock->locked = true;
//             return;
//         }
//     }
// }


// void klock_release( klock_t *lock )
// {
//     lock->locked = false;
// }





// // KProcess::KProcess( size_t id, const char *nm, void (*fn)(void*), void *arg )
// // :   pid      (id),
// //     priority (1),
// //     status   (PROC_READY),
// //     next     (nullptr)
// // {
// //     strncpy(name, nm, 64);

// //     uint8_t *stack = (uint8_t*)kmalloc(4096);
// //     uint8_t *top   = stack + 4096;
// //              top   = (uint8_t *)((uintptr_t)top & ~0xF);
// //              top -= sizeof(void *);
// //     memset(stack, 0, 4096);

// //     rsp = (uint64_t)top;
// //     rip = (uint64_t)fn;
// //     rdi = (uint64_t)arg;
// //     rbp = 0;
// // }




// // Scheduler::Scheduler()
// // :   m_curr (nullptr),
// //     m_pid  (0)
// // {
    
// // }



// // void
// // Scheduler::add_process( KProcess *proc )
// // {
// //     if (m_curr == nullptr)
// //     {
// //         m_curr = proc;
// //         m_curr->next = m_curr;
// //     }

// //     else if (m_curr == m_curr->next)
// //     {
// //         m_curr->next = proc;
// //         proc->next = m_curr;
// //     }

// //     else
// //     {
// //         proc->next = m_curr->next;
// //         m_curr->next = proc;
// //     }

// // }



// // KProcess*
// // Scheduler::createProcess( const char *name, void(*fn)(void*), void *arg )
// // {
// //     KProcess *proc = new KProcess(m_pid++, name, fn, arg);
// //     add_process(proc);

// //     return proc;
// // }


// // void
// // Scheduler::schedule( kstackframe *frame )
// // {
// //     static uint64_t ticks = 0;
// //     ticks += 1;

// //     if (m_curr == nullptr)
// //     {
// //         return;
// //     }

// //     if (m_curr->next != nullptr)
// //     {
// //         m_curr = m_curr->next;
// //     }

// //     frame->iret_rsp = m_curr->rsp;
// //     frame->iret_rip = m_curr->rip;
// //     frame->rdi      = m_curr->rdi;
// //     frame->rbp      = m_curr->rbp;

// // }


// // void
// // Scheduler::sleep( size_t pid )
// // {


// // }




// // KProcess *curr_process;

// // void proc_setmain( KProcess *P )
// // {
// //     curr_process = P;
// // }



// // // void add_process( KProcess *P )
// // // {
// // //     P->next = processes->next;
// // //     processes->next = P;
// // // }



// // void proc_schedule( kstackframe *frame )
// // {
// //     if (curr_process == nullptr)
// //     {
// //         return;
// //     }

// //     if (curr_process->next != nullptr)
// //     {
// //         curr_process = curr_process->next;
// //     }

// //     kstackframe *f2 = frame;

// //     f2->iret_rsp = curr_process->frame.iret_rsp;
// //     f2->iret_rip = curr_process->frame.iret_rip;
// //     f2->rdi      = curr_process->frame.rdi;
// //     f2->rbp      = curr_process->frame.rbp;
// // }


// // void idle_main( void* arg )
// // {
// //     while (true)
// //         asm("hlt");
// // }



// // #define KERNEL_CS (0x08 | 0)
// // #define KERNEL_SS (0x10 | 0)
// // #define user_cs   (0x18 | 3)
// // #define user_ss   (0x20 | 3)



// // size_t next_pid = 0;

// // KProcess*
// // create_process( const char *name, void(*function)(void*), void *arg )
// // {

// // }









// // size_t next_tid = 0;

// // thread_t *
// // add_thread( KProcess *proc, void(*function)(void*), void *arg)
// // {
// //     thread_t *thread = (thread_t*)kmalloc(sizeof(thread_t));

// //     if (proc->threads = nullptr)
// //     {
// //         proc->threads = thread;
// //     }

// //     else
// //     {
// //         for ( thread_t *scan = proc->threads; scan!=nullptr; scan=scan->next)
// //         {
// //             if (scan->next != nullptr)
// //                 continue;
// //             scan->next = thread;
// //             break;
// //         }
// //     }



// //     uint8_t *stack = (uint8_t*)kmalloc(4096);
// //     uint8_t *top   = stack + 4096;
// //              top   = (uint8_t *)((uintptr_t)top & ~0xF);
// //              top -= sizeof(void *);
// //     memset(stack, 0, 4096);


// //     // strncpy(thread->name, name, NAME_MAX_LEN);
// //     thread->tid = next_tid++;
// //     thread->status = READY;
// //     thread->next = nullptr;
// //     thread->frame.iret_ss = KERNEL_SS;
// //     thread->frame.iret_rsp = (uint64_t)stack;
// //     thread->frame.iret_flags = 0x202;
// //     thread->frame.iret_cs = KERNEL_CS;
// //     thread->frame.iret_rip = (uint64_t)function;
// //     thread->frame.rdi = (uint64_t)arg;
// //     thread->frame.rbp = 0;

// //     return thread;
// // }



