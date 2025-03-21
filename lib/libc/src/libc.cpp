#include <libc.h>
#include <stdio.h>
#include <ksysc.h>



int libc_init_stdio();


int libc_init()
{
    libc_init_stdio();
    return 1;
}

#ifdef __is_kernel
    #include <kfs.hpp>
#endif


int libc_init_stdio()
{
    FILE **files;

    #ifdef __is_kernel
        files = KFS::kstdio;

    #else
        ksysc_request req = {
            .type = SYSC_FILE_GETSTDIO
        };
        libk_syscall(&req);
        files = (FILE**)(req.data);

    #endif

    stderr = files[0];
    stdin  = files[1];
    stdout = files[2];
    // stdout = (FILE*)(req.data);

    return 1;
}

