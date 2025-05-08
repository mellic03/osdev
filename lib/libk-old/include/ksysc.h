#pragma once
#include "AAA.h"



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
    SYSC_MEM_REALLOC,
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

KERNEL_EXTERN_ASM void libk_syscall( struct ksysc_request *req );


#include "ZZZ.h"





