#include "kthread.hpp"
#include <kmalloc.h>
#include <kmemxx.h>
#include <kintcode.h>
#include <kinterrupt.h>
#include <kinplace/inplace_vector.hpp>

using namespace idk;



// extern "C"
// {
//     extern void __KThread_yield( KThread *next );

// }




// static inplace_vector<KThread*> threads;



// // void KThread_add( KThread *thread )
// // {
// //     if (threadlist == nullptr)
// //     {
// //         threadlist = thread;
// //         threadlist->next = threadlist;
// //         return;
// //     }

// //     thread->next = threadlist->next;
// //     threadlist->next = thread;
// // }


// void KThread_yield()
// {
//     syslog log("KThread_yield");
//     __kthread_switch(KThread_curr, KThread_curr->next);
// }


// void KThread_start()
// {
//     // KThread_curr = threadlist;
//     KThread_yield();
//     // ((threadfn_type)KThread_curr->rip)();
// }




// void KThread_exit()
// {
//     while (1) { printf("done"); }
// }


// void prisonA()
// {
//     while (1)
//     {
//         printf("[prisonA]\n");
//     }
// }



// void prisonB()
// {
//     while (1)
//     {
//         printf("[prisonB]\n");
//         KThread_yield();
//     }
// }



// void KThread_new( KThread *thread, threadfn_type fn, int idx )
// {
//     // uint8_t *stack_top = stack + stack_size;
//     // stack_top = (uint8_t *)((uintptr_t)stack_top & ~0xF);

//     // // Push fake return address
//     // stack_top -= sizeof(void *);
//     // *(void **)stack_top = (void*)KThread_exit; // Return address (null or KThread_exit function
//     memset(thread->k_stack, 0, sizeof(thread->k_stack));

//     if (idx == 0)
//         thread->k_stack[4096-1] = (uint64_t)(&prisonA);

//     else
//         thread->k_stack[4096-1] = (uint64_t)(&prisonB);

//     thread->rsp_val = &(thread->k_stack[4096-17]);
//     thread->cr3 = __cpu_get_cr3();
//     // thread->rbx = 0;
//     // thread->rbp = 0;
//     // thread->r12 = 0;
//     // thread->r13 = 0;
//     // thread->r14 = 0;
//     // thread->r15 = 0;
//     thread->next = nullptr;
// }


// void work_A()
// {
//     while (1)
//     {
//         printf("[Thread A]\n");
//     }
// }


// void work_B()
// {
//     while (1)
//     {
//         printf("[Thread B]\n");
//     }
// }


// void work_C()
// {
//     while (1)
//     {
//         printf("[Thread C]\n");
//     }
// }




// KThread A, B, C;

// void KThread_init()
// {
//     KThread_new(&A, work_A, 1);
//     KThread_new(&B, work_B, 0);
//     KThread_new(&C, work_C, 0);

//     A.next = &B;
//     B.next = &C;
//     C.next = &A;

//     KThread_curr = &A;

//     while (1)
//     {
//         // __kthread_switch(KThread_curr, KThread_curr->next);
//         KThread_yield();
//     }
// }


// // void switch_thread()
// // {
// //     KThread *next = KThread_curr->next;

// //     while (cand != current_process && cand->state != QUEUED) {
// //         cand = cand->next;
// //     }
// //     if (cand == current_process) {
// //         // went around all processes. Nobody was in QUEUED state!
// //         kprintf("No process found for running. Restarting shell!\n");
// //         create_process("bin/sbush");
// //         return;
// //     }

// //     if (current_process->state == RUNNING) {
// //         current_process->state = QUEUED;
// //     }
// //     else if (current_process->state != ZOMBIE) {
// //         kprintf("Some problem with process states: %d!! \n", current_process->state);
// //         return;
// //     }
// //     cand->state = RUNNING;
// //     set_new_cr3(cand->cr3);
// //     set_tss_rsp(&cand->k_stack[K_STACK_SIZE - 1]);
// //     __kthread_switch(&current_process, cand);
// // }