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



extern "C"
{
    // extern uint64_t test_jump_retvalue;
    // extern void test_main_jump( void );
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
    // syslog::kprintf("test: %lu", __kTCB_curr.tid);
    KInterrupt<INT_KTHREAD_START>();
}


#include <kernel/clock.hpp>

void kthread::sleep( uint64_t ms )
{
    kthread::m_curr->timer = 0;
    kthread::m_curr->timeToWake = ms;
    KInterrupt<INT_KTHREAD_YIELD>();
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

    // log("jump test A");
    // test_main_jump();
    // log("jump test result: %lu", test_jump_retvalue);


    if (m_curr == nullptr)
        kpanic("Cannot call kthread::start before creating any threads!");
    if (kthread::running)
        kpanic("Cannot call kthread::start more than once!");

    uint64_t frame_cs = frame->iret_cs;
    uint64_t frame_ss = frame->iret_ss;

    *frame = kthread::m_curr->frame;
    frame->iret_cs = frame_cs;
    frame->iret_ss = frame_ss;

    kthread::running = true;
}


void kthread::schedule( kstackframe *frame )
{
    // if (kthread::lock_count.load() != 0)
        // return;
    if (kthread::running == false)
        return;
    if (m_curr == nullptr)
        return;

    uint64_t frame_cs = frame->iret_cs;
    uint64_t frame_ss = frame->iret_ss;

    kthread *curr = m_curr;
    kthread *next = m_curr->next;

    curr->frame = *frame;
    *frame = next->frame;

    frame->iret_cs = frame_cs;
    frame->iret_ss = frame_ss;

    m_curr = m_curr->next;

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



#include <kmalloc.h>

kthread::kthread( const char thread_name[], void (*fn)(void*), void *arg )
{
    // std::lock_guard lock(kthread::global_lock);
    static size_t curr_tid = 0;
    memset(this->name, '\0', sizeof(this->name));
    strcpy(this->name, thread_name);

    size_t   size = 16*1024; // 16KB
    uint8_t *stack = (uint8_t*)kmalloc(size);
    uint8_t *top   = stack + size - 16;
             top   = (uint8_t *)((uintptr_t)top & ~0xF);
             top  -= sizeof(uint64_t);
    memset(stack, 0, size);

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
    kfree(this->stack);
}

