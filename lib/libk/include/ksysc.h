#pragma once
#include <AAA.h>



enum SYSC_
{
    SYSC_NONE          = 0,
    SYSC_FILE_CREATE   = 1,
    SYSC_FILE_DELETE   = 2,
    SYSC_FILE_RENAME   = 3,
    SYSC_FILE_WRITE    = 4,
    SYSC_FILE_GET,
    SYSC_FILE_FLUSH,
    SYSC_FILE_GETSTDIO,

    SYSC_MEM_ALLOC,
    SYSC_MEM_FREE,
};


enum SYSF_
{
    SYSF_FILE_ANY      = 1,
    SYSF_FILE_KDEVKEY,
    SYSF_FILE_KDEVSCN,

};


struct ksysc_request
{
    uint32_t type;
    uint32_t flags;
    uint64_t size;
    void    *data;
};


struct ksysc_response
{
    uint32_t type;
    uint32_t flags;
    uint64_t size;
    void    *data;
};

struct ksysc_file_request
{
    uint32_t type;
    uint64_t size;
    const void *data;
};

struct ksysc_stdio_request
{
    uint32_t type;
    uint32_t flags;
    char    *buf;
    const char *fmt;
    va_list *vlist;
};


#include <ZZZ.h>


extern "C"
{
    inline void libk_syscall( struct ksysc_request *req )
    {
        __asm__ volatile (
        "mov %0, %%rax\n\t"  // Move the pointer to rax
        "int $99\n\t"        // Trigger interrupt 99
        :                    // No output operands
        : "r" (req)          // Input operand
        : "rax"              // Clobbered register
    );

    }
}

