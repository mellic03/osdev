#include <kthread.hpp>
#include <kernel/log.hpp>
#include <kpanic.h>
#include <kintcode.h>
#include <kinterrupt.h>
#include <stdio.h>
#include <string.h>


// std::atomic_int kthread::refc::m_ct = 0;
kthread *kthread::m_curr    = nullptr;
bool     kthread::running   = false;



// extern "C"
// {
//     extern void __kthread_start( void );
//     extern void __kthread_yield( void );
// }


// extern "C"
// void kthread_start( kthread_frame *frame )
// {
//     __kthread_yield();
//     return;

//     syslog log("kthread_start");

//     log("&__kthread_start: 0x%lx", &__kthread_start);
//     log("__kthread_start:  0x%lx", __kthread_start);
//     // for (int i=0; i<13; i++)
//     // {
//     //     log("regs[%d]: 0x%lx", i, frame->regs[i]);
//     // }

//     log("rbp: 0x%lx", frame->rbp);
//     log("rsi: 0x%lx", frame->rsi);
//     log("rdi: 0x%lx", frame->rdi);
//     log("rdx: 0x%lx", frame->rdx);
//     log("rcx: 0x%lx", frame->rcx);
//     log("rbx: 0x%lx", frame->rbx);
//     log("rax: 0x%lx", frame->rax);
//     log("r15: 0x%lx", frame->r15);
//     log("r14: 0x%lx", frame->r14);
//     log("r13: 0x%lx", frame->r13);
//     log("r12: 0x%lx", frame->r12);
//     log("r11: 0x%lx", frame->r11);
//     log("rsp: 0x%lx", frame->rsp);
//     log("rip: 0x%lx", frame->rip);


//     kthread *&curr = kthread::m_curr;

//     frame->rax   = curr->rax;
//     frame->rbx   = curr->rbx;
//     frame->rcx   = curr->rcx;
//     frame->rdx   = curr->rdx;
//     frame->rsp   = curr->rsp;
//     frame->rip   = curr->rip;
//     // frame->flags = curr->flags;
//     frame->rdi   = curr->rdi;
//     frame->rsi   = curr->rsi;
//     frame->rbp   = curr->rbp;

//     kthread::running = true;

//     // kpanic("fuckywucky");
// }




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
    // __kthread_start();
    // static kthread TedBundy(kthread::ted_bundy, nullptr);
    // static kthread Idle(kthread::idlemain, nullptr);
    KInterrupt<INT_KTHREAD_START>();
}

void kthread::sleep( uint64_t )
{
    kthread::refc();
    // ct.wait();
    // m_curr->lastTimeAwake = now
}


void kthread::yield()
{
    // __kthread_yield();
    KInterrupt<INT_KTHREAD_YIELD>();
}


void kthread::exit()
{
    m_curr->status = KPROC_DEAD;
}


void kthread::start_handler( kstackframe *frame )
{
    kthread::refc();

    syslog log("start_handler");
    // log("global_lock: %d", global_lock);

    if (m_curr == nullptr)
        kpanic("Cannot call kthread::start before creating any threads!");
    if (kthread::running)
        kpanic("Cannot call kthread::start more than once!");

    frame->rax      = m_curr->rax;
    frame->rbx      = m_curr->rbx;
    frame->rcx      = m_curr->rcx;
    frame->rdx      = m_curr->rdx;
    frame->iret_rsp = m_curr->rsp;
    frame->iret_rip = m_curr->rip;
    frame->iret_flags = m_curr->flags;
    frame->rdi      = m_curr->rdi;
    frame->rsi      = m_curr->rsi;
    frame->rbp      = m_curr->rbp;

    kthread::running = true;
}




// extern "C"
// void kthread_switch( kstackframe *frame )
// {
//     // syslog log("kthread_switch");

//     kthread *&curr = kthread::m_curr;
//     kthread *next = curr->next;

//     curr->rax = frame->rax;
//     curr->rbx = frame->rbx;
//     curr->rcx = frame->rcx;
//     curr->rdx = frame->rdx;
//     curr->rsp = frame->iret_rsp;
//     curr->rip = frame->iret_rip;
//     curr->rdi = frame->rdi;
//     curr->rsi = frame->rsi;
//     curr->rbp = frame->rbp;
//     curr->flags = frame->iret_flags;

//     frame->rax = next->rax;
//     frame->rbx = next->rbx;
//     frame->rcx = next->rcx;
//     frame->rdx = next->rdx;
//     frame->iret_rsp = next->rsp;
//     frame->iret_rip = next->rip;
//     frame->iret_flags = next->flags;
//     frame->rdi = next->rdi;
//     frame->rsi = next->rsi;
//     frame->rbp = next->rbp;

//     curr = curr->next;
// }


void kthread::schedule( kstackframe *frame )
{
    if (!kthread::refc::try_wait())
        return;
    if (kthread::running == false)
        return;
    if (m_curr == nullptr)
        return;

    kthread *curr = m_curr;
    kthread *next = m_curr->next;

    curr->rax   = frame->rax;
    curr->rbx   = frame->rbx;
    curr->rcx   = frame->rcx;
    curr->rdx   = frame->rdx;
    curr->rsp   = frame->iret_rsp;
    curr->rip   = frame->iret_rip;
    curr->rdi   = frame->rdi;
    curr->rsi   = frame->rsi;
    curr->rbp   = frame->rbp;
    curr->flags = frame->iret_flags;

    frame->rax        = next->rax;
    frame->rbx        = next->rbx;
    frame->rcx        = next->rcx;
    frame->rdx        = next->rdx;
    frame->iret_rsp   = next->rsp;
    frame->iret_rip   = next->rip;
    frame->iret_flags = next->flags;
    frame->rdi        = next->rdi;
    frame->rsi        = next->rsi;
    frame->rbp        = next->rbp;

    m_curr = m_curr->next;
    
}



// static void kproc_idle( void* )
// {
//     while (true)
//     {
//         printf("idle\n");
//         asm volatile ("nop");
//     }
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


void kthread::ted_bundy( void* )
{
    printf("void [kthread::ted_bundy](void*)\n");

    while (true)
    {
        kthread::refc();
        kthread *th = m_curr->next;

        while (th != m_curr)
        {
            if (th->status == KPROC_DEAD)
            {
                printf("[kthread::ted_bundy] removing process %d\n", th->tid);
                kthread::remove(th);
                break;
            }
            th = th->next;
        }

        kthread::yield();
    }
}



kthread::kthread( void (*fn)(void*), void *arg )
{
    kthread::refc();
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
    this->flags = 0x202;
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

