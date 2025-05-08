#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __is_kernel
    #include <kmalloc.h>
#else
    #include <kernel/syscall.h>
#endif



void *malloc( size_t size )
{
    #ifdef __is_kernel
        return kmalloc(size);

    #else
        static sysreq_MemAlloc req;
        static sysres_t res;
        req = sysreq_MemAlloc(size);
        syscall(SysNo_MemAlloc, (sysreq_t*)(&req), &res);
        return res.addr;

    #endif
}


void *realloc( void *ptr, size_t size )
{
    #ifdef __is_kernel
        return krealloc(ptr, size);

    #else
        // static ksysc_request req;
        // req = {
        //     .type = SYSC_MEM_REALLOC,
        //     .size = size,
        //     .data = ptr
        // };
        // libk_syscall(&req);
        // return req.res;

    #endif
}



void free( void *ptr )
{
    #ifdef __is_kernel
        kfree(ptr);

    #else
        // static ksysc_request req;
        // req = {
        //     .type = SYSC_MEM_FREE,
        //     .data = ptr
        // };
        // libk_syscall(&req);

    #endif

}

