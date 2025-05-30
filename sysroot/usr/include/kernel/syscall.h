#pragma once
#include <stdint.h>

enum SysNo_
{
    SysNo_Invalid = 0,
    SysNo_FileOpen,
    SysNo_FileRead,
    SysNo_FileWrite,
    SysNo_MemAlloc,
    SysNo_MemFree,
    SysNo_SysNoCount,
};


struct sysreq_t
{
    uint64_t size;
    uint8_t  data[8];

    sysreq_t(): size(0) {  };
    sysreq_t( uint64_t sz ): size(sz) {  };
};

struct sysres_t
{
    void    *addr;
};


struct sysreq_FileOpen: sysreq_t
{
    uint64_t size  = sizeof(sysreq_FileOpen);
    uint64_t nbytes;
    char     path[8];
};


struct sysreq_MemAlloc: sysreq_t
{
    uint64_t size;
    uint64_t nbytes;

    sysreq_MemAlloc(): sysreq_MemAlloc(0) {  };

    sysreq_MemAlloc( uint64_t sz )
    :   sysreq_t(sizeof(sysreq_MemAlloc)),
        nbytes(sz) {  };
};


// namespace knl
// {
    void syscall( SysNo_ sysno, const sysreq_t *req, sysres_t *res );
// }

// #ifdef __cplusplus
//     extern "C" void *syscall( uint64_t sysno, void* );
// #else
// #endif




// enum SYSF_
// {
//     SYSF_FILE_ANY = 1,
//     SYSF_FILE_KDEVKEY,
//     SYSF_FILE_KDEVSCN,

// };


// struct ksysc_request
// {
//     uint32_t type;
//     uint32_t flags;
//     uint64_t size;
//     char     msg[64];
//     void    *data;
//     void    *res;
// };


// struct ksysc_window_response
// {
//     void *sde_ctx;
//     int *x, *y;
//     float *border;
// };

// KERNEL_EXTERN_ASM void libk_syscall( struct ksysc_request *req );


// #include "ZZZ.h"





