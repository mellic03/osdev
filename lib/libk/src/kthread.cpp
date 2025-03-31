#include <kthread.hpp>
#include <kernel/log.hpp>
#include <kpanic.h>
#include <kintcode.h>
#include <kinterrupt.h>
#include <stdio.h>
#include <string.h>


std::atomic_int kthread::lock_count = 0;
kthread *kthread::m_curr  = nullptr;
bool     kthread::running = false;


extern "C"
{
    extern kstackframe context_arg;
    extern kstackframe context_arg2;
    extern void kthread_start( void );
    extern void kthread_yield( void );
}

extern "C"
void kthread_switch( uint64_t *rsp )
{
    kthread::yield_lock();

    syslog log("kthread_switch");

    // kthread *curr = kthread::m_curr;
    // kthread *next = kthread::m_curr->next;
    auto *frame = (kthread_frame*)rsp;

    for (int i=0; i<15; i++)
    {
        log("rsp[%d]: 0x%lx", i, rsp[i]);
    }

    log("rbp:        0x%lx",  frame->rbp);
    log("rsi:        0x%lx",  frame->rsi);
    log("rdi:        0x%lx",  frame->rdi);
    log("rdx:        0x%lx",  frame->rdx);
    log("rcx:        0x%lx",  frame->rcx);
    log("rbx:        0x%lx",  frame->rbx);
    log("rax:        0x%lx",  frame->rax);
    log("r15:        0x%lx",  frame->r15);
    log("r14:        0x%lx",  frame->r14);
    log("r13:        0x%lx",  frame->r13);
    log("r12:        0x%lx",  frame->r12);
    log("r11:        0x%lx",  frame->r11);
    log("ret_rsp:    0x%lx",  frame->rsp);
    log("ret_rip:    0x%lx",  frame->rip);

    // log("ret_rsp:    0x%lx",  frame->rsp);
    // log("ret_rip:    0x%lx",  frame->rip);
    // log("r11:        0x%lx",  frame->r11);
    // log("r12:        0x%lx",  frame->r12);
    // log("r13:        0x%lx",  frame->r13);
    // log("r14:        0x%lx",  frame->r14);
    // log("r15:        0x%lx",  frame->r15);
    // log("rax:        0x%lx",  frame->rax);
    // log("rbx:        0x%lx",  frame->rbx);
    // log("rcx:        0x%lx",  frame->rcx);
    // log("rdx:        0x%lx",  frame->rdx);
    // log("rdi:        0x%lx",  frame->rdi);
    // log("rsi:        0x%lx",  frame->rsi);
    // log("rbp:        0x%lx",  frame->rbp);


    // frame->r11 = curr->r11;
    // frame->r12 = curr->r12;
    // frame->r13 = curr->r13;
    // frame->r14 = curr->r14;
    // frame->r15 = curr->r15;
    // frame->rax = curr->rax;
    // frame->rbx = curr->rbx;
    // frame->rcx = curr->rcx;
    // frame->rdx = curr->rdx;
    // frame->rdi = curr->rdi;
    // frame->rsi = curr->rsi;
    // frame->rbp = curr->rbp;
    // frame->rip = curr->rip;
    // frame->rsp = curr->rsp;

    kthread::m_curr = kthread::m_curr->next;
}



extern "C"
void __kthread_start( uint64_t *rsp )
{
    syslog log("__kthread_start");

    for (int i=0; i<15; i++)
    {
        log("rsp[%d]: 0x%lx", i, rsp[i]);
    }

    kthread *curr = kthread::m_curr;
    // kthread *next = kthread::m_curr->next;
    auto *frame = (kthread_frame*)rsp;

    log("r11:        0x%lx",  frame->r11);
    log("r12:        0x%lx",  frame->r12);
    log("r13:        0x%lx",  frame->r13);
    log("r14:        0x%lx",  frame->r14);
    log("r15:        0x%lx",  frame->r15);
    log("rax:        0x%lx",  frame->rax);
    log("rbx:        0x%lx",  frame->rbx);
    log("rcx:        0x%lx",  frame->rcx);
    log("rdx:        0x%lx",  frame->rdx);
    log("rdi:        0x%lx",  frame->rdi);
    log("rsi:        0x%lx",  frame->rsi);
    log("rbp:        0x%lx",  frame->rbp);
    log("ret_rip:    0x%lx",  frame->rip);
    log("ret_rsp:    0x%lx",  frame->rsp);

    frame->r11 = curr->r11;
    frame->r12 = curr->r12;
    frame->r13 = curr->r13;
    frame->r14 = curr->r14;
    frame->r15 = curr->r15;
    frame->rax = curr->rax;
    frame->rbx = curr->rbx;
    frame->rcx = curr->rcx;
    frame->rdx = curr->rdx;
    frame->rdi = curr->rdi;
    frame->rsi = curr->rsi;
    frame->rbp = curr->rbp;
    frame->rip = curr->rip;
    frame->rsp = curr->rsp;

    kthread::running = true;
}



extern "C"
void __kthread_yield( uint64_t *rsp )
{
    kthread::yield_lock();
    syslog log("__kthread_yield");

    for (int i=0; i<15; i++)
    {
        log("rsp[%d]: 0x%lx", i, rsp[i]);
    }

    kthread *curr = kthread::m_curr;
    kthread *next = kthread::m_curr->next;
    auto *frame = (kthread_frame*)rsp;

    log("r11:        0x%lx",  frame->r11);
    log("r12:        0x%lx",  frame->r12);
    log("r13:        0x%lx",  frame->r13);
    log("r14:        0x%lx",  frame->r14);
    log("r15:        0x%lx",  frame->r15);
    log("rax:        0x%lx",  frame->rax);
    log("rbx:        0x%lx",  frame->rbx);
    log("rcx:        0x%lx",  frame->rcx);
    log("rdx:        0x%lx",  frame->rdx);
    log("rdi:        0x%lx",  frame->rdi);
    log("rsi:        0x%lx",  frame->rsi);
    log("rbp:        0x%lx",  frame->rbp);
    log("ret_rip:    0x%lx",  frame->rip);
    log("ret_rsp:    0x%lx",  frame->rsp);

    curr->r11 = frame->r11;
    curr->r12 = frame->r12;
    curr->r13 = frame->r13;
    curr->r14 = frame->r14;
    curr->r15 = frame->r15;
    curr->rax = frame->rax;
    curr->rbx = frame->rbx;
    curr->rcx = frame->rcx;
    curr->rdx = frame->rdx;
    curr->rdi = frame->rdi;
    curr->rsi = frame->rsi;
    curr->rbp = frame->rbp;
    curr->rip = frame->rip;
    curr->rsp = frame->rsp;

    frame->r11 = next->r11;
    frame->r12 = next->r12;
    frame->r13 = next->r13;
    frame->r14 = next->r14;
    frame->r15 = next->r15;
    frame->rax = next->rax;
    frame->rbx = next->rbx;
    frame->rcx = next->rcx;
    frame->rdx = next->rdx;
    frame->rdi = next->rdi;
    frame->rsi = next->rsi;
    frame->rbp = next->rbp;
    frame->rip = next->rip;
    frame->rsp = next->rsp;

    kthread::m_curr = kthread::m_curr->next;

    kpanic("Boye");
    // kthread::m_curr = kthread::m_curr->next;
}




void kthread::idlemain( void* )
{
    while (true)
    {
        kthread::yield();
    }
}


size_t kthread::this_tid()
{
    return m_curr->tid;
}


void kthread::start()
{
    kthread_start();
}

void kthread::sleep( uint64_t )
{
    kthread::yield_lock();
    // ct.wait();
    // m_curr->lastTimeAwake = now
}


void kthread::yield()
{
    kthread_yield();
}


void kthread::exit()
{
    m_curr->status = KPROC_DEAD;
}


// void kthread::schedule( kstackframe *frame )
// {
//     if (kthread::lock_count.load() > 0)
//         return;
//     if (kthread::running == false)
//         return;
//     if (m_curr == nullptr)
//         return;


// }




void kthread::wrapper( void (*fn)(void*), void *arg )
{
    fn(arg);
    m_curr->status = KPROC_DEAD;
    printf("[kthread::wrapper] finished\n");
    while (true) { asm volatile ("hlt"); }
}



void kthread::add( kthread *th )
{
    if (m_curr == nullptr)
    {
        m_curr = th;
        m_curr->next = m_curr;
    }

    else if (m_curr == m_curr->next)
    {
        m_curr->next = th;
        th->next = m_curr;
    }

    else
    {
        th->next = m_curr->next;
        m_curr->next = th;
    }
}


void kthread::remove( kthread *th ) 
{
    if (th->next == nullptr)
    {
        kpanic("next == nullptr");
    }

    kthread *prev = nullptr;
    kthread *curr = th->next;

    while (curr != th)
    {
        prev = curr;
        curr = curr->next;
    }

    if (prev == nullptr)
    {
        kpanic("prev == nullptr");
    }

    prev->next = th->next;
    delete th;
}


// void kthread::ted_bundy( void* )
// {
//     printf("void [kthread::ted_bundy](void*)\n");

//     while (true)
//     {
//         kthread::yield_lock();
//         kthread *th = m_curr->next;

//         while (th != m_curr)
//         {
//             if (th->status == KPROC_DEAD)
//             {
//                 printf("[kthread::ted_bundy] removing process %d\n", th->tid);
//                 kthread::remove(th);
//                 break;
//             }
//             th = th->next;
//         }

//         kthread::yield();
//     }
// }



kthread::kthread( void (*fn)(void*), void *arg )
{
    kthread::yield_lock();
    static size_t curr_tid = 0;

    uint8_t *stack = new uint8_t[4096];
    uint8_t *top   = stack + 4096;
             top   = (uint8_t *)((uintptr_t)top & ~0xF);
             top  -= sizeof(uint64_t);
    memset(stack, 0, 4096);

    this->tid    = curr_tid++;
    this->status = KPROC_READY;
    this->stack  = stack;
    this->rsp = (uint64_t)top;
    this->rip = (uint64_t)kthread::wrapper;
    this->rdi = (uint64_t)fn;
    this->rsi = (uint64_t)arg;
    this->rbp = 0;
    this->next = nullptr;

    syslog log("kthread::kthread");
    log("tid: %lu", this->tid);

    kthread::add(this);
}


kthread::~kthread()
{
    printf("[kthread::~kthread]\n");
    delete[] this->stack;
}

