#include <libc.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __is_kernel
    #include <kmalloc.h>
#endif


static ksysc_request req;


void *malloc( size_t nbytes )
{
    #ifdef __is_kernel
        return kmalloc(nbytes);

    #else
        req = {
            .type = SYSC_MEM_ALLOC,
            .size = nbytes
        };
        libk_syscall(&req);
        return req.data;

    #endif
}


void free( void *ptr )
{
    #ifdef __is_kernel
        kfree(ptr);

    #else
        req = {
            .type = SYSC_MEM_FREE,
            .data = ptr
        };

        libk_syscall(&req);

    #endif

}

