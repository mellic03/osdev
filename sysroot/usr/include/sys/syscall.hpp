#pragma once

#pragma once
#include <stdint.h>

enum SysNo_
{
    SysNo_Invalid = 0,
    SysNo_Exit,
    SysNo_ShellExec,
    SysNo_FileOpen,
    SysNo_FileRead,
    SysNo_FileWrite,
    SysNo_MemAlloc,
    SysNo_MemFree,
    SysNo_PrintTest,
    SysNo_SysNoCount,
};


// struct sysreq_FileOpen: sysreq_t
// {
//     uint64_t size  = sizeof(sysreq_FileOpen);
//     uint64_t nbytes;
//     char     path[8];
// };


// struct sysreq_MemAlloc: sysreq_t
// {
//     uint64_t size;
//     uint64_t nbytes;

//     sysreq_MemAlloc(): sysreq_MemAlloc(0) {  };

//     sysreq_MemAlloc( uint64_t sz )
//     :   sysreq_t(sizeof(sysreq_MemAlloc)),
//         nbytes(sz) {  };
// };


namespace knl
{
    struct sysreq_t
    {
        uint64_t size;
        uint8_t  data[8];

        sysreq_t(): size(0) {  };
        sysreq_t( uint64_t sz ): size(sz) {  };
    };

    struct sysres_t
    {
        void *addr;
    };

    void syscall( SysNo_ sysno, const void *req, void *res );
}

