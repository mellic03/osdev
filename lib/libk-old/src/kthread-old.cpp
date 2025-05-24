// #include <kthread.hpp>
// #include <kernel/clock.hpp>
// #include <kernel/log.hpp>
// #include <kpanic.h>
// #include <kernel/interrupt.hpp>
// #include <stdio.h>
// #include <string.h>
// #include <kernel/log.hpp>

// kthread *kthread::m_curr    = nullptr;
// bool     kthread::running   = false;

// #include <inplace_vector.hpp>

// idk::inplace_vector<kthread> kthread_all;

// // static void idlemain( void* )
// // {
// //     while (true)
// //     {
// //         for (auto *th: kthread_all)
// //         {
// //             int64_t delta = kclock::uptime_msecs.load() - th->timeLastAwake;
        
// //             if (delta > th->timeToSleep)
// //             {
// //                 th->timeToSleep = 0;
// //                 th->status = KPROC_READY;
// //             }
// //         }
// //     }
// // }


// // size_t kthread::this_tid()
// // {
// //     return m_curr->tid;
// // }


// void kthread::start()
// {
//     // syslog::print("test: %lu", __kTCB_curr.tid);
//     KInterrupt<IntNo_KTHREAD_START>();
// }

// void kthread::sleep( uint64_t ms )
// {
//     kthread::m_curr->timeLastAwake = kclock::uptime_msecs.load();
//     kthread::m_curr->sleepDuration = ms;
//     kthread::m_curr->status = KPROC_SLEEPING;
//     KInterrupt<IntNo_KTHREAD_YIELD>();
// }

// void kthread::yield()
// {
//     KInterrupt<IntNo_KTHREAD_YIELD>();
// }

// void kthread::exit()
// {
//     m_curr->status = KPROC_DEAD;
// }


// void kthread::start_handler( kstackframe *frame )
// {
//     syslog log("start_handler");

//     if (m_curr == nullptr)
//         kpanic("Cannot call kthread::start before creating any threads!");
//     if (kthread::running)
//         kpanic("Cannot call kthread::start more than once!");

//     uint64_t frame_cs = frame->iret_cs;
//     uint64_t frame_ss = frame->iret_ss;

//     asm volatile("fxsave %0 " : : "m"(kthread::m_curr->m_fxdata));

//     *frame = kthread::m_curr->frame;
//     frame->iret_cs = frame_cs;
//     frame->iret_ss = frame_ss;

//     kthread::running = true;
// }


// void kthread::schedule( kstackframe *frame )
// {
//     // if (kthread::lock_count.load() != 0)
//         // return;
//     if (kthread::running == false)
//         return;
//     if (m_curr == nullptr)
//         return;

//     uint64_t frame_cs = frame->iret_cs;
//     uint64_t frame_ss = frame->iret_ss;

//     kthread *curr = m_curr;
//     kthread *next = m_curr->next;

//     while (next->status == KPROC_SLEEPING)
//     {
//         int64_t delta = kclock::uptime_msecs.load() - next->timeLastAwake;
//         if (delta > next->sleepDuration)
//         {
//             next->sleepDuration = 0;
//             next->status = KPROC_READY;
//         }
//         next = next->next;
//     }

//     asm volatile("fxsave %0 " : : "m"(curr->m_fxdata));

//     curr->status = KPROC_READY;
//     next->status = KPROC_RUNNING;

//     curr->frame = *frame;
//     *frame = next->frame;

//     frame->iret_cs = frame_cs;
//     frame->iret_ss = frame_ss;

//     asm volatile("fxrstor %0 " : : "m"(next->m_fxdata));

//     m_curr = next;
// }



// void kthread::wrapper( void (*fn)(void*), void *arg )
// {
//     fn(arg);
//     m_curr->status = KPROC_DEAD;
//     printf("[kthread::wrapper] finished\n");
//     while (true) { asm volatile ("hlt"); }
// }



// void kthread::add( kthread *th )
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


// void kthread::remove( kthread *th ) 
// {
//     if (th->next == nullptr)
//     {
//         kpanic("next == nullptr");
//     }

//     kthread *prev = nullptr;
//     kthread *curr = th->next;

//     while (curr != th)
//     {
//         prev = curr;
//         curr = curr->next;
//     }

//     if (prev == nullptr)
//     {
//         kpanic("prev == nullptr");
//     }

//     prev->next = th->next;
//     delete th;
// }


// // void kthread::ted_bundy( void* )
// // {
// //     syslog::print("void [kthread::ted_bundy](void*)\n");

// //     while (true)
// //     {
// //         kthread *th = m_curr->next;

// //         while (th != m_curr)
// //         {
// //             if (th->status == KPROC_DEAD)
// //             {
// //                 syslog::print("[kthread::ted_bundy] removing process %d\n", th->tid);
// //                 kthread::remove(th);
// //                 break;
// //             }
// //             th = th->next;
// //         }

// //         kthread::yield();
// //     }
// // }



// #include <kmalloc.h>

// kthread::kthread( const char thread_name[], void (*fn)(void*), void *arg )
// {
//     static size_t curr_tid = 0;
//     memset(this->name, '\0', sizeof(this->name));
//     strcpy(this->name, thread_name);

//     size_t   size = 64*1024; // 64KB
//     uint8_t *stack = (uint8_t*)kmalloc(size);
//     uint8_t *top   = stack + size - 16;
//              top   = (uint8_t *)((uintptr_t)top & ~0xF);
//              top  -= sizeof(uint64_t);
//     memset(stack, 0, size);

//     this->tid    = curr_tid++;
//     this->status = KPROC_READY;
//     this->stack  = stack;
//     this->frame.iret_rsp = (uint64_t)top;
//     this->frame.iret_rip = (uint64_t)kthread::wrapper;
//     this->frame.rdi = (uint64_t)fn;
//     this->frame.rsi = (uint64_t)arg;
//     this->frame.rbp = 0;
//     this->frame.iret_rflags = 0x202;
//     this->next = nullptr;

//     kthread::add(this);
// }


// // kthread::~kthread()
// // {
// //     // printf("[kthread::~kthread]\n");
// //     kfree(this->stack);
// // }

