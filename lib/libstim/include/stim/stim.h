#pragma once
#include "interrupt.h"
#include "syscall.h"


#ifdef __cplusplus
extern "C" {
#endif


    void stim_makefile( const char *path );
    void stim_writefile( const char *path, const void *data, uint32_t nbytes );

    void stim_putstr( const char *str );


#ifdef __cplusplus
}
#endif

