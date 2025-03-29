#pragma once
#include <AAA.h>



enum SYSC_
{
    SYSC_NONE = 0,
    SYSC_FILE_CREATE,
    SYSC_FILE_DELETE,
    SYSC_FILE_RENAME,
    SYSC_FILE_WRITE,
    SYSC_FILE_GET,
    SYSC_FILE_FLUSH,

    SYSC_WINDOW_CREATE,
    SYSC_WINDOW_DELETE,

    SYSC_MEM_ALLOC,
    SYSC_MEM_FREE,
};


enum SYSF_
{
    SYSF_FILE_ANY = 1,
    SYSF_FILE_KDEVKEY,
    SYSF_FILE_KDEVSCN,

};


struct ksysc_request
{
    uint32_t type;
    uint32_t flags;
    uint64_t size;
    char     msg[64];
    void    *data;
    void    *res;
};


struct ksysc_window_response
{
    void *sde_ctx;
    int *x, *y;
    float *border;
};



#include <ZZZ.h>


extern "C"
{
    extern void libk_syscall( struct ksysc_request *req );

    // inline void libk_syscall( struct ksysc_request *req )
    // {
    //     __asm__ volatile (
    //         "mov %0, %%rdi\n\t"  // Move the pointer to rax
    //         "int $99\n\t"        // Trigger interrupt 99
    //         :                    // No output operands
    //         : "r" (req)          // Input operand
    //         : "rdi"              // Clobbered register
    //     );
    // }
}

