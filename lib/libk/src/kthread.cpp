#include <kthread.hpp>
#include <kpanic.h>
#include <kintcode.h>
#include <kinterrupt.h>
#include <stdio.h>
#include <string.h>
// #include <kernel/log.hpp>


void kthread::global_lock()
{
    asm volatile ("cli");
    m_global_lock = true;
}


void kthread::global_unlock()
{
    m_global_lock = false;
	asm volatile ("sti");
}



size_t kthread::this_tid()
{
    size_t tid = 0;

    kthread::global_lock();
    tid = m_curr->tid;
    kthread::global_unlock();

    return tid;
}

void kthread::yield()
{
    KInterrupt<INT_KTHREAD_YIELD>();
}


void kthread::exit()
{
    m_curr->status = KPROC_DEAD;
}




void kthread::schedule( kstackframe *frame )
{
    if (m_global_lock == true)
    {
        return;
    }

    if (m_curr == nullptr)
    {
        return;
    }

    if (m_first == false)
    {
        m_curr->rax = frame->rax;
        m_curr->rbx = frame->rbx;
        m_curr->rcx = frame->rcx;
        m_curr->rdx = frame->rdx;
        m_curr->rsp = frame->iret_rsp;
        m_curr->rip = frame->iret_rip;
        m_curr->rdi = frame->rdi;
        m_curr->rsi = frame->rsi;
        m_curr->rbp = frame->rbp;
        m_curr->flags = frame->iret_flags;
    }

    m_first = false;
    m_curr = m_curr->next;

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
    kthread::global_lock();

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

    kthread::global_unlock();
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
    static bool first = true;
    if (first)
    {
        first = false;
        new kthread(kthread::ted_bundy, nullptr);
    }

    kthread::global_lock();

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

    kthread::add(this);
    kthread::global_unlock();
}


kthread::~kthread()
{
    printf("[kthread::~kthread]\n");
    delete[] this->stack;
}



void klock_acquire( klock_t *lock )
{
    while (true)
    {
        if (lock->locked == false)
        {
            lock->locked = true;
            return;
        }
    }
}


void klock_release( klock_t *lock )
{
    lock->locked = false;
}



