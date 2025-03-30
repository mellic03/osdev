#include <kthread.hpp>
#include <kernel/log.hpp>
#include <kpanic.h>
#include <kintcode.h>
#include <kinterrupt.h>
#include <stdio.h>
#include <string.h>



size_t kthread::this_tid()
{
    // kthread::lock_guard();
    return m_curr->tid;
}


void kthread::start()
{
    KInterrupt<INT_KTHREAD_START>();

}


void kthread::yield()
{
    KInterrupt<INT_KTHREAD_YIELD>();
}


void kthread::exit()
{
    m_curr->status = KPROC_DEAD;
}


void kthread::start_handler( kstackframe *frame )
{
    kthread::lock_guard();

    // syslog log("start_handler");
    // log("global_lock: %d", global_lock);

    if (m_curr == nullptr)
        kpanic("Cannot call kthread::start before creating any threads!");
    if (m_started != false)
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

    m_started = true;
}


void kthread::schedule( kstackframe *frame )
{
    // syslog log("kthread::schedule");
    // log("global_lock: %d", global_lock);

    if (kthread::global_lock.load() != 0)
        return;
    if (m_started == false)
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



static void kproc_idle( void* )
{
    while (true)
    {
        printf("idle\n");
        asm volatile ("nop");
    }
}


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
    kthread::lock_guard();

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
    while (true)
    {
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
    kthread::lock_guard();
    // static bool first = true;
    // if (first)
    // {
    //     first = false;
    //     new kthread(kthread::ted_bundy, nullptr);
    // }

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

