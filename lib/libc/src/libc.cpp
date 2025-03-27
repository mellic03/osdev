#include <libc.h>
#include <stdio.h>
#include <ksysc.h>

#ifdef __is_kernel
    #include <kfs.hpp>
#endif



int libc_init()
{
    libc_init_stdio();
    return 1;
}


static ksysc_request req;

int libc_init_stdio()
{
    FILE **files;

    #ifdef __is_kernel
        files = KFS::kstdio;

    #else
        req = {
            .type = SYSC_FILE_GETSTDIO
        };
        libc_syscall(&req);
        files = (FILE**)(req.data);

    #endif

    stderr = files[0];
    stdin  = files[1];
    stdout = files[2];

    return 1;
}

