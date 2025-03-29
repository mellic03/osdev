#include <libc.h>
#include <stdio.h>
#include <string.h>
#include <ksysc.h>

#ifdef __is_kernel
    #include <kernel/vfs.hpp>
#endif

int libc_init()
{
    libc_init_stdio();
    return 1;
}


static ksysc_request req;

int libc_init_stdio()
{
    #ifdef __is_kernel
        stdout = (FILE*)(kfilesystem::vfsFindFile("dev/stdio"));
    
    #else
        req.type = SYSC_FILE_GET;
        strcpy(req.msg, "dev/stdout");
        libc_syscall(&req);
        stdout = (FILE*)(req.data);

    #endif

    return 1;
}

