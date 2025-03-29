#include <kthread.hpp>
#include <kmalloc.h>
#include <kintcode.h>
#include <kinterrupt.h>
#include <stdio.h>
#include <string.h>
// #include <kernel/log.hpp>


void kthread::global_lock()
{
    klock_acquire(&m_global_lock);
}


void kthread::global_unlock()
{
    klock_release(&m_global_lock);
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
    kthread::global_lock();
    m_curr->status = KPROC_DEAD;
    kthread::global_unlock();
}




void kthread::schedule( kstackframe *frame )
{
    if (m_global_lock.locked)
    {
        return;
    }

    if (m_curr == nullptr)
    {
        return;
    }

    if (m_curr->next == nullptr)
    {
        return;
    }

    if (m_curr->next->status == KPROC_DEAD)
    {
        kthread *th = m_curr->next;
        m_curr->next = m_curr->next->next;
        // delete th;
        // kproc_free(P);
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


kthread::kthread( void (*fn)(void*), void *arg )
{
    asm volatile ("cli");
    static size_t curr_tid = 0;

    uint8_t *stack = (uint8_t*)kmalloc(4096);
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
	asm volatile ("sti");
}


kthread::~kthread()
{
    // kfree(this->stack);
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



