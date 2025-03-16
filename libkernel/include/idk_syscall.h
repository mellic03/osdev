#pragma once
#include <AAA.h>



enum SysRequest_: uint32_t
{
    SysRequest_NONE          = 0,
    SysRequest_FILE_CREATE   = 1,
    SysRequest_FILE_DELETE   = 2,
    SysRequest_FILE_RENAME   = 3,
    SysRequest_FILE_GETSTDIO = 4
};
enum SysResponse_: uint32_t
{
    SysResponse_NONE     = 0,
    SysResponse_FAILURE  = 1,
    SysResponse_SUCCESS  = 2
};


void idk_Syscall( uint32_t *type, uint32_t *flags, char buf[256] );




#include <AAB.h>
