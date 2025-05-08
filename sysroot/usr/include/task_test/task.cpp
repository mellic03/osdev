#include "task.hpp"
#include <kmalloc.h>
// #include "../memory/paging.h"
// #include "../memory/pmm.h"
// #include "../kernel.h"

extern void TaskTest_Switch( taskframe_t *from, taskframe_t *to );
 
// void yield() 
// {
//     Task *last = runningTask;
//     runningTask = runningTask->next;
//     switchTask(&last->regs, &runningTask->regs);
// }

// void doIt() {
// 	term_write("Switching to otherTask... \n", 27);
// 	yield();
// 	term_write("good\n",5);
// }

void createTask( void (*entry)(), uint64_t flags, uint64_t cr3 )
{
    taskframe_t regs;
    regs.rax = 0;
    regs.rbx = 0;
    regs.rcx = 0;
    regs.rdx = 0;
    regs.rsi = 0;
    regs.rdi = 0;
    regs.rflags = flags;
    regs.rip = (uint64_t)entry;
    regs.cr3 = cr3;
    regs.rsp = (uint64_t)kmalloc(4096) + 0x1000;
    // next = NULL;
}


#include <cpu/cpu.hpp>

static void otherMain()
{

}

void initTasking() 
{
    uint64_t cr3 = CPU::getCR3();
    uint64_t rflags = CPU::getRFLAGS();

    createTask(otherMain, rflags, cr3);

    // mainTask.next = &otherTask;
    // otherTask.next = &mainTask;
    // runningTask = &mainTask;
}