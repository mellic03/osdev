#include <kproc.hpp>
#include <kmalloc.h>
#include <kintcode.h>
#include <kinterrupt.h>
#include <stdio.h>
#include <string.h>
// #include "../log/log.hpp"


klock_t  global_lock;
kproc_t *m_curr  = nullptr;
bool     m_first;



size_t kthread::this_tid()
{
    size_t tid = 0;

    klock_acquire(&global_lock);
    tid = m_curr->tid;
    klock_release(&global_lock);

    return tid;
}

void kthread::yield()
{
    KInterrupt<INT_PROCESS_SWITCH>();
}


void kthread::exit()
{
    klock_acquire(&global_lock);
    m_curr->status = KPROC_DEAD;
    klock_release(&global_lock);
}


void kthread::create( void (*fn)(void*), void *arg )
{
    kproc_new(fn, arg);
}





void kproc_free( kproc_t *P )
{
    kfree(P->stack);
    kfree(P);
}


void kproc_switch( kstackframe *frame )
{
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
        auto *P = m_curr->next;
        m_curr->next = m_curr->next->next;
        kproc_free(P);
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

    m_curr = m_curr->next;
    m_first = false;

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


void kproc_schedule( kstackframe *frame )
{
    kproc_switch(frame);
}

void kproc_yield_irq( kstackframe *frame )
{
    kproc_switch(frame);
}







static void kproc_idle( void* )
{
    while (true)
    {
        printf("idle\n");
        asm volatile ("nop");
    }
}


void kproc_init()
{
    global_lock.locked = false;
    m_curr = nullptr;
    m_first = true;

    // auto *idle = kproc_new(kproc_idle, nullptr);
    // kproc_detach(idle);
}



void kproc_yield()
{
    KInterrupt<INT_PROCESS_SWITCH>();
}

static void process_wrapper( void (*fn)(void*), void *arg )
{
    fn(arg);
    m_curr->status = KPROC_DEAD;
    while (true) { asm volatile ("hlt"); }
}





static void __kproc_add( kproc_t *th )
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


kproc_t *kproc_new( void (*fn)(void*), void *arg )
{
    asm volatile ("cli");
    static size_t tid = 0;

    uint8_t *stack = (uint8_t*)kmalloc(4096);
    uint8_t *top   = stack + 4096;
             top   = (uint8_t *)((uintptr_t)top & ~0xF);
             top  -= sizeof(uint64_t);
    memset(stack, 0, 4096);


    kproc_t *thread = (kproc_t*)kmalloc(sizeof(kproc_t));

    thread->tid    = tid++;
    thread->status = KPROC_READY;
    thread->stack  = stack;
    thread->rsp = (uint64_t)top;
    thread->rip = (uint64_t)process_wrapper;
    thread->rdi = (uint64_t)fn;
    thread->rsi = (uint64_t)arg;
    thread->rbp = 0;
    thread->flags = 0x202;
    thread->next = nullptr;

    __kproc_add(thread);
	asm volatile ("sti");
 
    return thread;
}


// void kproc_detach( kproc_t *thread )
// {

// }




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





// KProcess::KProcess( size_t id, const char *nm, void (*fn)(void*), void *arg )
// :   pid      (id),
//     priority (1),
//     status   (PROC_READY),
//     next     (nullptr)
// {
//     strncpy(name, nm, 64);

//     uint8_t *stack = (uint8_t*)kmalloc(4096);
//     uint8_t *top   = stack + 4096;
//              top   = (uint8_t *)((uintptr_t)top & ~0xF);
//              top -= sizeof(void *);
//     memset(stack, 0, 4096);

//     rsp = (uint64_t)top;
//     rip = (uint64_t)fn;
//     rdi = (uint64_t)arg;
//     rbp = 0;
// }




// Scheduler::Scheduler()
// :   m_curr (nullptr),
//     m_pid  (0)
// {
    
// }



// void
// Scheduler::add_process( KProcess *proc )
// {
//     if (m_curr == nullptr)
//     {
//         m_curr = proc;
//         m_curr->next = m_curr;
//     }

//     else if (m_curr == m_curr->next)
//     {
//         m_curr->next = proc;
//         proc->next = m_curr;
//     }

//     else
//     {
//         proc->next = m_curr->next;
//         m_curr->next = proc;
//     }

// }



// KProcess*
// Scheduler::createProcess( const char *name, void(*fn)(void*), void *arg )
// {
//     KProcess *proc = new KProcess(m_pid++, name, fn, arg);
//     add_process(proc);

//     return proc;
// }


// void
// Scheduler::schedule( kstackframe *frame )
// {
//     static uint64_t ticks = 0;
//     ticks += 1;

//     if (m_curr == nullptr)
//     {
//         return;
//     }

//     if (m_curr->next != nullptr)
//     {
//         m_curr = m_curr->next;
//     }

//     frame->iret_rsp = m_curr->rsp;
//     frame->iret_rip = m_curr->rip;
//     frame->rdi      = m_curr->rdi;
//     frame->rbp      = m_curr->rbp;

// }


// void
// Scheduler::sleep( size_t pid )
// {


// }




// KProcess *curr_process;

// void proc_setmain( KProcess *P )
// {
//     curr_process = P;
// }



// // void add_process( KProcess *P )
// // {
// //     P->next = processes->next;
// //     processes->next = P;
// // }



// void proc_schedule( kstackframe *frame )
// {
//     if (curr_process == nullptr)
//     {
//         return;
//     }

//     if (curr_process->next != nullptr)
//     {
//         curr_process = curr_process->next;
//     }

//     kstackframe *f2 = frame;

//     f2->iret_rsp = curr_process->frame.iret_rsp;
//     f2->iret_rip = curr_process->frame.iret_rip;
//     f2->rdi      = curr_process->frame.rdi;
//     f2->rbp      = curr_process->frame.rbp;
// }


// void idle_main( void* arg )
// {
//     while (true)
//         asm("hlt");
// }



// #define KERNEL_CS (0x08 | 0)
// #define KERNEL_SS (0x10 | 0)
// #define user_cs   (0x18 | 3)
// #define user_ss   (0x20 | 3)



// size_t next_pid = 0;

// KProcess*
// create_process( const char *name, void(*function)(void*), void *arg )
// {

// }









// size_t next_tid = 0;

// thread_t *
// add_thread( KProcess *proc, void(*function)(void*), void *arg)
// {
//     thread_t *thread = (thread_t*)kmalloc(sizeof(thread_t));

//     if (proc->threads = nullptr)
//     {
//         proc->threads = thread;
//     }

//     else
//     {
//         for ( thread_t *scan = proc->threads; scan!=nullptr; scan=scan->next)
//         {
//             if (scan->next != nullptr)
//                 continue;
//             scan->next = thread;
//             break;
//         }
//     }



//     uint8_t *stack = (uint8_t*)kmalloc(4096);
//     uint8_t *top   = stack + 4096;
//              top   = (uint8_t *)((uintptr_t)top & ~0xF);
//              top -= sizeof(void *);
//     memset(stack, 0, 4096);


//     // strncpy(thread->name, name, NAME_MAX_LEN);
//     thread->tid = next_tid++;
//     thread->status = READY;
//     thread->next = nullptr;
//     thread->frame.iret_ss = KERNEL_SS;
//     thread->frame.iret_rsp = (uint64_t)stack;
//     thread->frame.iret_flags = 0x202;
//     thread->frame.iret_cs = KERNEL_CS;
//     thread->frame.iret_rip = (uint64_t)function;
//     thread->frame.rdi = (uint64_t)arg;
//     thread->frame.rbp = 0;

//     return thread;
// }



