// #include <cpu/cpu.hpp>
// #include <smp/smp.hpp>
// #include <kthread.hpp>
// #include <kernel/clock.hpp>
// #include <kernel/log.hpp>
// #include <kthread.hpp>
// #include <kernel/bitmanip.hpp>
// #include <kmalloc.h>
// #include <kassert.h>
// #include <string.h>


// // static std::mutex global_sched_lock;


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
// //     // auto &active   = sd->m_active;
// //     auto &sleeping = sd->m_sleeping;

// //     for (int i=0; i<int(sleeping.size()); i++)
// //     {
// //         kthread_t *th = sleeping[i];
// //         if (kclock::now() >= th->wakeTime)
// //         {
// //             th->status = KThread_READY;
// //             vec_remove(sleeping, th);
// //             // vec_insert(sd.t, th);
// //             i -= 1;
// //         }
// //     }
// // }



// // static void threadManager( void* )
// // {
// //     while (true)
// //     {
// //         // auto *cpu = SMP::this_cpu();
// //         // syslog::print("[CPU%u threadManager]\n", cpu->id);
        
// //         auto *sd = SMP::this_sched();
// //         threadManager_UpdateDead(sd);
// //         threadManager_UpdateSleeping(sd);
// //         kthread::yield();
// //     }
// // }


// // static void cullmain( void* )
// // {
// //     while (true)
// //     {
// //         asm volatile ("hlt");
// //     }
// // }


// static void kthread_wrapper( void (*tmain)(void*), void *arg )
// {
//     asm volatile ("sti");
//     tmain(arg);
//     kthread::yield();
//     while (true) { CPU::hlt(); }
// }

// static void idlemain( void* )
// {
//     while (true)
//     {
//         asm volatile ("hlt");
//     }
// }


// ThreadScheduler::ThreadScheduler()
// {
//     m_startLock.set();
//     m_switchCount.store(0);
//     m_threads.clear();
//     m_sleeping.clear();

//     m_idlethread = addThread("idlemain", idlemain, nullptr);
//     // kassert("No cpu", cpu != nullptr);
// }


// /*
//     https://github.com/LemonOSProject/LemonOS/blob/15f607e8be86fcaf9c86a68c91915e820c9e935e/Kernel/src/Arch/x86_64/Thread.cpp#L48
// */

// #include <kernel/memory/pmm.hpp>
// #include <kernel/memory/vmm.hpp>

// struct fxstate_t
// {
//     uint16_t fcw; // FPU Control Word
//     uint16_t fsw; // FPU Status Word
//     uint8_t ftw;  // FPU Tag Words
//     uint8_t zero; // Literally just contains a zero
//     uint16_t fop; // FPU Opcode
//     uint64_t rip;
//     uint64_t rdp;
//     uint32_t mxcsr;      // SSE Control Register
//     uint32_t mxcsrMask;  // SSE Control Register Mask
//     uint8_t st[8][16];   // FPU Registers, Last 6 bytes reserved
//     uint8_t xmm[16][16]; // XMM Registers
// } __attribute__((packed));



// kthread_t*
// ThreadScheduler::addThread( const char *name, void (*fn)(void*), void *arg )
// {
//     // auto *th = (kthread_t*)VMM::alloc(sizeof(kthread_t));
//     auto *th = (kthread_t*)kmalloc(sizeof(kthread_t));
//           th->setPriority(5);
//     m_threads.push_back(th);

//     memset(th->stack, 0, sizeof(th->stack));
//     memset(th->fxstate, 0, sizeof(th->fxstate));
//     memset(&(th->frame), 0, sizeof(intframe_t));
//     memset(th->name, '\0', sizeof(th->name));
//     strncpy(th->name, name, sizeof(th->name));

//     th->useSSE       = false;
//     th->tid          = m_thread_tid++;
//     th->status       = KThread_READY;
//     th->wakeTime     = 0;
//     th->stackTop     = idk::align_down(th->stack + sizeof(th->stack), 16);

//     th->frame.rsp    = (uint64_t)(th->stackTop);
//     th->frame.rip    = (uint64_t)kthread_wrapper;
//     th->frame.rdi    = (uint64_t)fn;
//     th->frame.rsi    = (uint64_t)arg;
//     th->frame.rbp    = 0;
//     th->frame.rflags = CPU::getRFLAGS(); // 0x202;

//     return th;
// }


// void
// ThreadScheduler::releaseThread( kthread_t *th )
// {
//     // VMM::free(th, sizeof(kthread_t));
//     kfree(th);
// }




// void
// ThreadScheduler::trampoline( intframe_t *frame )
// {
//     uint64_t frame_cs = frame->cs;
//     uint64_t frame_ss = frame->ss;

//     auto *curr = m_threads.front();

//     *frame = curr->frame;
//     frame->cs = frame_cs;
//     frame->ss = frame_ss;

//     m_startLock.clear();
// }



// /**
//  * @return Next thread
//  */
// kthread_t *swap_threads( kthread_t *curr, kthread_t *next, intframe_t *frame )
// {
//     uint64_t tmp_cs = frame->cs;
//     uint64_t tmp_ss = frame->ss;

//     curr->status = KThread_READY;
//     next->status = KThread_RUNNING;

//     curr->frame = *frame;
//     *frame = next->frame;

//     frame->cs = tmp_cs;
//     frame->ss = tmp_ss;

//     // // if (curr->useSSE)
//     //     CPU::fxsave(curr->fxstate);
//     // // if (next->useSSE)
//     //     CPU::fxrstor(next->fxstate);

//     return next;
// }



// void
// ThreadScheduler::check_sleepers()
// {
//     size_t size = m_sleeping.size();

//     for (size_t i=0; i<size; i++)
//     {
//         if (m_sleeping.empty())
//             return;

//         kthread_t *curr = m_sleeping.front();
//         if (kclock::now() >= curr->wakeTime)
//         {
//             curr->status = KThread_READY;
//             curr = m_sleeping.pop_front();
//             m_threads.push_back(curr);
//         }

//         else
//         {
//             m_sleeping.rotate(1);
//         }
//     }
// }


// void
// ThreadScheduler::schedule( intframe_t *frame )
// {
//     // m_switchCount++;

//     // while (m_switchCount.load() > 0)
//     // {
//     //     CPU::nop();
//     // }
//     if (m_switchCount++ > 0)
//     {
//         m_switchCount--;
//         return;
//     }

//     // syslog::println("[schedule] m_threads.size(): %lu", m_threads.size());

//     kthread_t *prev = nullptr;
//     kthread_t *next = nullptr;
//     prev = m_threads.front();

//     if (prev->status != KThread_SLEEPING)
//     {
//         m_threads.rotate(1);
//         next = m_threads.front();
//     }

//     else
//     {
//         prev = m_threads.pop_front();
//         m_sleeping.push_back(prev);
//         next = m_threads.front();
//     }

//     if (next == m_idlethread)
//     {
//         m_threads.rotate(1);
//         next = m_threads.front();
//     }

//     check_sleepers();

//     // syslog::println(
//     //     "[ThreadScheduler::schedule CPU%lu] %s --> %s",
//     //     SMP::this_cpuid(), prev->name, next->name
//     // );

    
//     // if (prev->useSSE)
//         CPU::fxsave(prev->fxstate);

//     swap_threads(prev, next, frame);

//     // if (next->useSSE)
//         CPU::fxrstor(next->fxstate);

//     m_switchCount--;
// }







// // void ThreadScheduler::trampolineISR( intframe_t *frame )
// // {
// //     syslog::println("[trampolineISR] cpu=0x%lx", SMP::this_cpu());
// //     // syslog::println("[CPU%lu trampolineISR]", SMP::this_cpu()->id);
// //     SMP::this_sched()->trampoline(frame);
// // }


// #include <kernel/log.hpp>
// void ThreadScheduler::scheduleISR( intframe_t *frame )
// {
//     auto *sd = SMP::this_sched();
//     if (sd->m_startLock.isset())
//         sd->trampoline(frame);
//     else
//         sd->schedule(frame);
// }

