#include <cpu/cpu.hpp>
#include <cpu/cpu.hpp>

#include <kernel/interrupt.hpp>
#include <kthread.hpp>

#include <kernel/clock.hpp>
#include <kernel/interrupt.hpp>
#include <kernel/log.hpp>

#include <kmalloc.h>
#include <kpanic.h>
#include <string.h>

#include <inplace_vector.hpp>



void kthread::yield()
{
    KInterrupt<IntNo_KTHREAD_YIELD>();
}


void kthread::sleep( uint64_t ms )
{
    auto *th = SMP::this_thread();
    th->wakeTime = kclock::now() + ms;
    th->status   = KThread_SLEEPING;
    kthread::yield();
}


void kthread::exit()
{
    auto *th = SMP::this_thread();
    th->status = KThread_DEAD;
    kthread::yield();
}


kthread_t *kthread::create( const char *name, void (*fn)(void*), void *arg )
{
    static int idx = 0;

    auto &cpu = SMP::all_cpus[idx];
    idx = (idx+1) % SMP::num_cpus;

    return cpu.sched.addThread(name, fn, arg);
}


// void kthread::start()
// {
//     auto *cpu = SMP::this_cpu();
//     syslog::println("[kthread::start] cpu=0x%lx", cpu);
//     if (cpu) cpu->sched.start();
// }


// void kthread::yield()
// {
//     KInterrupt<IntNo_KTHREAD_YIELD>();
// }

// void kthread::exit()
// {
//     // allThreads[currThread].status = kthread_Dead;
// }

// // uint8_t kthread::this_tid()
// // {
// //     return currThread;
// // }

// // kthread_t *kthread::this_thread()
// // {
// //     return allThreads[currThread];
// // }


// void kthread::start_handler( kstackframe *frame )
// {
//     syslog log("kthread::start_handler");

//     // kassert("No threads to start", allThreads.size() > 0);
//     // kassert("Ruh roh", hasStarted.load() == true);

//     uint64_t frame_cs = frame->iret_cs;
//     uint64_t frame_ss = frame->iret_ss;

//     auto *curr = allThreads[currThread];
//     asm volatile("fxsave %0 " : : "m"(curr->fxdata));

//     *frame = curr->frame;
//     frame->iret_cs = frame_cs;
//     frame->iret_ss = frame_ss;

//     log("currThread: %u", currThread);

//     kthread::isRunning = true;
// }



// static void removeThread( uint8_t idx ) 
// {
//     kthread_t *th = allThreads[idx];
//     allThreads[idx] = allThreads.back();
//     allThreads.pop_back();
//     // allThreads[idx] = allThreads[numThreads-1];
//     // numThreads -= 1;

//     if (idx == currThread)
//     {
//         currThread = (currThread+1) % allThreads.size();
//     }

//     kfree(th);
//     // kfree(th->fxdata);
// }





// void kthread::schedule( kstackframe *frame )
// {
//     // if (kthread::lock_count.load() != 0)
//         // return;
//     if (kthread::isRunning == false)
//     {
//         return;
//     }

//     // for (size_t i=0; i<numThreads; i++)
//     for (size_t i=0; i<allThreads.size(); i++)
//     {
//         if (allThreads[i]->status == kthread_Dead)
//         {
//             removeThread(i);
//             break;
//         }
//     }



//     uint8_t currIdx = currThread;
//     uint8_t nextIdx = (currIdx+1) % allThreads.size();
//     // uint8_t nextIdx = (currIdx+1) % numThreads;

//     auto &curr = allThreads[currIdx];
//     auto &next = allThreads[nextIdx];

//     kassert("Ruh roh", curr->status!=kthread_Dead && next->status!=kthread_Dead);

//     // while (allThreads[nextIdx].status == KPROC_SLEEPING)
//     // {
//     //     auto &next = allThreads[nextIdx];

//     //     int64_t delta = kclock::uptime_msecs.load() - next.timeLastAwake;
//     //     if (delta > next.sleepDuration)
//     //     {
//     //         next.sleepDuration = 0;
//     //         next.status = kthread_Ready;
//     //     }
//     //     nextIdx = (nextIdx+1) % allThreads.size();
//     // }


//     curr->status = kthread_Ready;
//     next->status = kthread_Running;

//     uint64_t frame_cs = frame->iret_cs;
//     uint64_t frame_ss = frame->iret_ss;

//     curr->frame = *frame;
//     *frame = next->frame;

//     frame->iret_cs = frame_cs;
//     frame->iret_ss = frame_ss;

//     asm volatile("fxsave %0 " : : "m"(curr->fxdata));
//     asm volatile("fxrstor %0 " : : "m"(next->fxdata));

//     currThread = nextIdx;
// }



// static void kthread_wrapper( void (*fn)(void*), void *arg )
// {
//     fn(arg);
//     allThreads[currThread]->status = kthread_Dead;
//     printf("[kthread::wrapper] finished\n");
//     while (true) { asm volatile ("hlt"); }
// }



// uint8_t kthread::create( const char thread_name[], void (*fn)(void*), void *arg, uint8_t *fxstate )
// {
//     syslog log("kthread::create");
//     if (allThreads.capacity() == 0)
//     {
//         allThreads = idk::inplace_vector<kthread_t*>(
//             (kthread_t**)kmalloc(maxThreads*sizeof(kthread_t*)),
//             maxThreads
//         );
//     }

//     allThreads.push_back(new kthread_t());
//     auto *th = allThreads.back();
//     th->tid = allThreads.size() - 1;

//     memset(th->name, '\0', sizeof(th->name));
//     strncpy(th->name, thread_name, sizeof(th->name));
//     memset(th->stack, 0, sizeof(th->stack));
//     memcpy(th->fxdata, fxstate, 512*sizeof(uint8_t));

//     th->status           = kthread_Ready;
//     th->frame.iret_rsp   = (uint64_t)(th->stack_top);
//     th->frame.iret_rip   = (uint64_t)kthread_wrapper;
//     th->frame.rdi        = (uint64_t)fn;
//     th->frame.rsi        = (uint64_t)arg;
//     th->frame.rbp        = 0;
//     th->frame.iret_rflags = 0x202;

//     // th->fxdata = (uint8_t*)kmalloc(512 * sizeof(uint8_t));
//     // memcpy(th->fxdata, fxstate, 512*sizeof(uint8_t));
//     // asm volatile("fxsave %0 " : : "m"(th->fxdata));

//     return th->tid;
// }


// // static void idle_thread( void* )
// // {
// //     syslog log("idle_thread");

// //     while (true)
// //     {
// //         // for (size_t i=0; i<numThreads; i++)
// //         for (size_t i=0; i<allThreads.size(); i++)
// //         {
// //             if (allThreads[i].status == kthread_Dead)
// //             {
// //                 removeThread(i);
// //                 break;
// //             }
// //         }

// //         // kthread::sleep(1000);
// //         kthread::yield();
// //     }
// // }



