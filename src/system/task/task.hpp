#pragma once

#include <stdint.h>
#include "memory/memory.hpp"


namespace idk
{
    class TaskDispatcher;
    // TaskDispatcher &getTaskDispatcher();

    struct TaskRegisters
    {
        uint64_t rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, rip, rflags, cr3;
    };

    struct Task
    {
        TaskRegisters regs;
        struct Task *next;
    };

}


class idk::TaskDispatcher
{
private:
    uint64_t page_size = 0x1000;
    uint64_t pages[4];

public:
    idk::Task *runningTask;
    idk::Task  mainTask;
    idk::Task  otherTask;

    void init( linear_allocator& );
    void initTasking( void (*taskfn)() );

    void createTask( Task*, void(*)(), uint64_t, uint64_t*, uint64_t );
    void yield();
};



// extern void initTasking();
// extern void yield(); // Switch task frontend
