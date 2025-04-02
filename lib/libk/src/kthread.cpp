#include <kthread.hpp>
#include <kernel/log.hpp>
#include <kpanic.h>
#include <kintcode.h>
#include <kinterrupt.h>
#include <stdio.h>
#include <string.h>
#include <kernel/log.hpp>


std::mutex kthread::global_lock;
kthread *kthread::m_curr    = nullptr;
bool     kthread::running   = false;



// kthread::yield_guard::yield_guard()
// {
//     // kthread::lock_count++;
//     syslog::kprintf("[yield_guard] %d\n", kthread::lock_count.add_fetch(1));
// };

// kthread::yield_guard::~yield_guard()
// {
//     // kthread::lock_count--;
//     syslog::kprintf("[yield_guard] %d\n", kthread::lock_count.sub_fetch(1));
// };



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
    KInterrupt<INT_KTHREAD_START>();
}

void kthread::sleep( uint64_t )
{
    // std::lock_guard lock(kthread::global_lock);
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
    syslog log("start_handler");

    if (m_curr == nullptr)
        kpanic("Cannot call kthread::start before creating any threads!");
    if (kthread::running)
        kpanic("Cannot call kthread::start more than once!");

    auto *curr = kthread::m_curr;

    frame->rax        = curr->frame.rax;
    frame->rbx        = curr->frame.rbx;
    frame->rcx        = curr->frame.rcx;
    frame->rdx        = curr->frame.rdx;
    frame->iret_rsp   = curr->frame.iret_rsp;
    frame->iret_rip   = curr->frame.iret_rip;
    frame->iret_flags = curr->frame.iret_flags;
    frame->rdi        = curr->frame.rdi;
    frame->rsi        = curr->frame.rsi;
    frame->rbp        = curr->frame.rbp;

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
    // if (global_lock.try_lock() == false)
    // {
    //     return;
    // }

    if (kthread::lock_count.load() != 0)
    {
    //     // printf("\n\n---------------------------lmao\n\n");
        return;
    }
    if (kthread::running == false)
        return;
    if (m_curr == nullptr)
        return;

    kthread *curr = m_curr;
    kthread *next = m_curr->next;

    curr->frame.rax   = frame->rax;
    curr->frame.rbx   = frame->rbx;
    curr->frame.rcx   = frame->rcx;
    curr->frame.rdx   = frame->rdx;
    curr->frame.iret_rsp   = frame->iret_rsp;
    curr->frame.iret_rip   = frame->iret_rip;
    curr->frame.rdi   = frame->rdi;
    curr->frame.rsi   = frame->rsi;
    curr->frame.rbp   = frame->rbp;
    curr->frame.iret_flags = frame->iret_flags;

    frame->rax        = next->frame.rax;
    frame->rbx        = next->frame.rbx;
    frame->rcx        = next->frame.rcx;
    frame->rdx        = next->frame.rdx;
    frame->iret_rsp   = next->frame.iret_rsp;
    frame->iret_rip   = next->frame.iret_rip;
    frame->iret_flags = next->frame.iret_flags;
    frame->rdi        = next->frame.rdi;
    frame->rsi        = next->frame.rsi;
    frame->rbp        = next->frame.rbp;

    m_curr = m_curr->next;

    // global_lock.unlock();
    
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
        // std::lock_guard lock(kthread::global_lock);
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



kthread::kthread( const char thread_name[], void (*fn)(void*), void *arg )
{
    // std::lock_guard lock(kthread::global_lock);
    static size_t curr_tid = 0;
    memset(this->name, '\0', sizeof(this->name));
    strcpy(this->name, thread_name);

    uint8_t *stack = new uint8_t[4096];
    uint8_t *top   = stack + 4096 - 16;
             top   = (uint8_t *)((uintptr_t)top & ~0xF);
             top  -= sizeof(uint64_t);
    memset(stack, 0, 4096);

    this->tid    = curr_tid++;
    this->status = KPROC_READY;
    this->stack  = stack;
    this->frame.iret_rsp = (uint64_t)top;
    this->frame.iret_rip = (uint64_t)kthread::wrapper;
    this->frame.rdi = (uint64_t)fn;
    this->frame.rsi = (uint64_t)arg;
    this->frame.rbp = 0;
    this->frame.iret_flags = 0x202;
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

