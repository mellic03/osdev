#include <libc.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __is_kernel
    #include <kmalloc.h>
#endif




void *malloc( size_t size )
{
    #ifdef __is_kernel
        return kmalloc(size);

    #else
        static ksysc_request req;
        req = {
            .type = SYSC_MEM_ALLOC,
            .size = size
        };
        libk_syscall(&req);
        return req.res;

    #endif
}


void *realloc( void *ptr, size_t size )
{
    #ifdef __is_kernel
        return krealloc(ptr, size);

    #else
        static ksysc_request req;
        req = {
            .type = SYSC_MEM_REALLOC,
            .size = size,
            .data = ptr
        };
        libk_syscall(&req);
        return req.res;

    #endif
}



void free( void *ptr )
{
    #ifdef __is_kernel
        kfree(ptr);

    #else
        static ksysc_request req;
        req = {
            .type = SYSC_MEM_FREE,
            .data = ptr
        };
        libk_syscall(&req);

    #endif

}

