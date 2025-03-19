#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    
    enum SYSC_
    {
        SYSC_NONE          = 0,
        SYSC_FILE_CREATE   = 1,
        SYSC_FILE_DELETE   = 2,
        SYSC_FILE_RENAME   = 3,
        SYSC_FILE_WRITE    = 4,
        SYSC_FILE_GETSTDIO = 5,

        SYSC_MEM_ALLOC,
        SYSC_MEM_FREE,

        SYSC_PUT_STRING    = 5325
    };


    typedef struct
    {
        uint32_t type;
        uint32_t flags;
    } ksysc_request;

    typedef struct
    {
        uint32_t type;
        uint32_t nbytes;
        void    *data;
    } ksysc_file_request;

    typedef struct
    {
        uint32_t type;
        uint32_t flags;
        void    *data;
    } ksysc_io_request;

    // extern void stim_syscall( uint64_t*, uint64_t );
    extern void stim_syscall( ksysc_request* );

#ifdef __cplusplus
}
#endif

