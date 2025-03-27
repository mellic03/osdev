#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <ksysc.h>


extern "C"
{
    int libc_init();
    int libc_init_stdio();
    extern void libc_syscall( struct ksysc_request *req );
}


#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

