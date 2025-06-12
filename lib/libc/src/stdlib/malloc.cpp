#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __is_kernel
    #include <kmalloc.h>

    void *malloc( size_t size )
    {
        return kmalloc(size);
    }

    void *calloc( size_t num, size_t size )
    {
        void *ptr = malloc(num*size);
        memset(ptr, 0, num*size);
        return ptr;
    }

    void *realloc( void *ptr, size_t size )
    {
        return krealloc(ptr, size);
    }

    void free( void *ptr )
    {
        kfree(ptr);
    }



#else
    #include <kernel/syscall.h>
    void *malloc( size_t )
    {
        // static ksysc_request req;
        // req = {
        //     .type = SYSC_MEM_ALLOC,
        //     .data = ptr
        // };
        // libk_syscall(&req);
        return nullptr;
    }

    void *calloc( size_t num, size_t size )
    {
        void *ptr = malloc(num*size);
        memset(ptr, 0, num*size);
        return ptr;
    }

    void *realloc( void*, size_t )
    {
        return nullptr;
    }

    void free( void* )
    {
        // static ksysc_request req;
        // req = {
        //     .type = SYSC_MEM_FREE,
        //     .data = ptr
        // };
        // libk_syscall(&req);
    }

#endif

