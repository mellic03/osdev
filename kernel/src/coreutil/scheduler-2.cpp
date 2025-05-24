// #include <cpu/scheduler.hpp>
// #include <kthread.hpp>
// #include "kernel/clock.hpp"
// #include "kernel/log.hpp"
// #include <cpu/cpu.hpp>
// #include <kthread.hpp>
// #include <kassert.h>
// #include <kmalloc.h>
// #include <string.h>



// // static void threadManager_UpdateDead( ThreadScheduler *sd )
// // {
// //     auto &dead = sd->m_dead;

// //     while (dead.size() > 0)
// //     {
// //         auto *th = dead.back(); dead.pop_back();
// //         // syslog::print("[threadManager_UpdateDead] \"%s\"\n", th->name);
// //         vec_remove(sd->m_threads, th);
// //         sd->releaseThread(th);
// //     }
// // }

// // static void threadManager_UpdateSleeping( ThreadScheduler *sd )
// // {
// //     auto &threads = sd->m_sleeping;
// //     int  &count   = sd->m_sleepsize;

// //     for (int i=0; i<count; i++)
// //     {
// //         kthread_t *th = threads[i];

// //         if (kclock::now() >= th->wakeTime)
// //         {
// //             threads[i] = threads[count-1];
// //             count -= 1;
        
// //             sd->m_active.push_back(th);
// //             return;
// //         }
// //     }
// // }



// void kthread::cullmain( void* )
// {
//     while (true)
//     {
//         // cpu_t *cpu   = SMP::this_cpu();
//         // auto *sched = SMP::this_sched();
//         // size_t count = sched->m_active.size();
//         // syslog::println("[CPU%lu cullmain] count=%lu", cpu->id, count);

//         // threadManager_UpdateDead(sd);
//         // threadManager_UpdateSleeping(sd);
//         while (true) { asm volatile ("hlt"); }
//         // kthread::yield();
//     }
// }

// void kthread::idlemain( void* )
// {
//     while (true)
//     {
//         // cpu_t *cpu   = SMP::this_cpu();
//         // size_t count = cpu->sched->m_active.size();
//         // syslog::println("[CPU%lu idlemain] count=%lu", cpu->id, count);
//         while (true) { asm volatile ("hlt"); }
//         // kthread::yield();
//     }
// }










// static void kthread_wrapper( void (*fn)(void*), void *arg )
// {
//     // syslog::print("[kthread_wrapper] entry\n");
//     fn(arg);
//     // syslog::print("[kthread_wrapper] exit\n");

//     auto *th = SMP::this_thread();
//     th->status = KThread_DEAD;
//     kthread::yield(YldRsn_Dead);

//     while (true) { asm volatile ("hlt"); }
// }



// void ThreadScheduler::init( cpu_t *cpu )
// {
//     kassert("No cpu", (cpu != nullptr));

//     m_cpu = cpu;
//     // m_sleepsize = 0;
//     // for (int i=0; i<32; i++)
//     //     m_sleeping[i] = nullptr;
// }




// void
// ThreadScheduler::start()
// {
//     if (m_active.size() < 2)
//     {
//         syslog::println("[ThreadScheduler::start] BRUH WTF???");
//         kpanic("REEE");
//     }
//     KInterrupt<IntNo_KTHREAD_START>();
// }




// kthread_t*
// ThreadScheduler::createThread( const char *name, void (*fn)(void*), void *arg )
// {
//     auto *th = (kthread_t*)kmalloc(sizeof(kthread_t));

//     memset(th->stack, 0, sizeof(th->stack));
//     memset(th->name, '\0', sizeof(th->name));
//     strncpy(th->name, name, sizeof(th->name));

//     th->cpu              = m_cpu;
//     th->tid              = m_thread_tid++;
//     th->status           = KThread_ACTIVE;
//     th->wakeTime         = 0;
//     th->stackTop         = th->stack + sizeof(th->stack) - 16;

//     th->frame.iret_rsp   = (uint64_t)(th->stackTop);
//     th->frame.iret_rip   = (uint64_t)kthread_wrapper;
//     th->frame.rdi        = (uint64_t)fn;
//     th->frame.rsi        = (uint64_t)arg;
//     th->frame.rbp        = 0;
//     th->frame.iret_rflags = 0x202; // CPU::getRFLAGS() ??
//     // asm volatile("fxsave %0 " : : "m"(th->fxdata));

//     // syslog::println("[CPU%lu createThread] size A: %lu", m_cpu->id, m_active.size());
//     m_active.push_back(th);
//     // syslog::println("[CPU%lu createThread] size B: %lu\n", m_cpu->id, m_active.size());

//     return th;
// }

// void
// ThreadScheduler::releaseThread( kthread_t *th )
// {
//     kfree(th);
// }



// /**
//  * @return The new "current" thread.
//  */
// static kthread_t *swap_threads( kthread_t *curr, kthread_t *next, intframe_t *frame )
// {
//     uint64_t tmp_cs = frame->iret_cs;
//     uint64_t tmp_ss = frame->iret_ss;

//     // asm volatile("fxsave %0 " : : "m"(curr->fxdata));

//     curr->frame = *frame;
//     *frame = next->frame;

//     frame->iret_cs = tmp_cs;
//     frame->iret_ss = tmp_ss;

//     // asm volatile("fxrstor %0 " : : "m"(next->fxdata));

//     return next;
// }



// void
// ThreadScheduler::trampoline( intframe_t *frame )
// {
//     uint64_t frame_cs = frame->iret_cs;
//     uint64_t frame_ss = frame->iret_ss;

//     kthread_t *curr = m_active.front();
//     // asm volatile("fxsave %0 " : : "m"(curr->fxdata));

//     *frame = curr->frame;
//     frame->iret_cs = frame_cs;
//     frame->iret_ss = frame_ss;

//     m_curr = curr;
// }


// void
// ThreadScheduler::schedule( intframe_t *frame )
// {
//     if (m_active.empty())
//         return;
        
//     // syslog::println("[schedule] m_active.size(): %lu", m_active.size());

//     cpu_t *cpu = SMP::this_cpu();
//     auto   rsn = cpu->getYldRsn();

//     kthread_t *prev = m_active.pop_front();
//     kthread_t *next = m_active.front();
//     m_curr = swap_threads(prev, next, frame);

//     if (rsn == YldRsn_Dead)
//     {
//         releaseThread(prev);
//     }

//     // if (rsn == YldRsn_Dead)
//     // {
//     //     kthread_t *next = m_active.front();
//     //     m_curr = swap_threads(prev, next, frame);
//     //     releaseThread(prev);
//     // }

//     // else
//     // {
//     //     m_active.push_back(prev);
//     //     kthread_t *next = m_active.front();
//     //     m_curr = swap_threads(prev, next, frame);
//     // }


// }







// void ThreadScheduler::trampolineISR( intframe_t *frame )
// {
//     SMP::this_sched()->trampoline(frame);
// }


// void ThreadScheduler::scheduleISR( intframe_t *frame )
// {
//     SMP::this_sched()->schedule(frame);
// }

