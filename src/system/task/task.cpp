#include "task.hpp"

// idk::TaskDispatcher&
// idk::getTaskDispatcher()
// {
//     static idk::TaskDispatcher td;
//     return td;
// }



extern "C"
{
    extern void switchTask( idk::TaskRegisters *from, idk::TaskRegisters *to );
}



void
idk::TaskDispatcher::init( linear_allocator &mem )
{
    for (int i=0; i<4; i++)
    {
        uint8_t *ptr = mem.alloc<uint8_t>(page_size);
        pages[i] = (uint64_t)(ptr);
    }
}


void
idk::TaskDispatcher::initTasking( void (*taskfn)() )
{
    asm volatile("movq %%cr3, %%rax; movq %%rax, %0;" : "=m"(mainTask.regs.cr3) :: "%rax");
    asm volatile("pushfq; movq (%%rsp), %%rax; movq %%rax, %0; popfq;" : "=m"(mainTask.regs.rflags) :: "%rax");
    
    createTask(
        &otherTask, taskfn,
        mainTask.regs.rflags, 
        (uint64_t*)mainTask.regs.cr3,
        pages[1]
    );

    mainTask.next  = &otherTask;
    otherTask.next = &mainTask;
    runningTask    = &mainTask;
}


void
idk::TaskDispatcher::createTask( Task *task, void (*main)(), uint64_t flags,
                                 uint64_t *pagedir, uint64_t page )
{
    task->regs.rax = 0;
    task->regs.rbx = 0;
    task->regs.rcx = 0;
    task->regs.rdx = 0;
    task->regs.rsi = 0;
    task->regs.rdi = 0;
    task->regs.rflags = flags;
    task->regs.rip = (uint64_t)main;
    task->regs.cr3 = (uint64_t)pagedir;
    task->regs.rsp = page;
    task->next = 0;
}


void
idk::TaskDispatcher::yield()
{
    Task *last = runningTask;
    runningTask = runningTask->next;
    switchTask(&last->regs, &runningTask->regs);
}