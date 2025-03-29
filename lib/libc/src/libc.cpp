#include <libc.h>
#include <stdio.h>
#include <string.h>
#include <ksysc.h>

int libc_init()
{
    libc_init_stdio();
    return 1;
}










#ifdef __is_kernel
    #include <kernel/ioport.hpp>
    #include <kernel/vfs.hpp>

    static void stdout_flush( kfstream *fh )
    {
        uint8_t *base   = fh->m_base;
        uint8_t *&read  = fh->m_read;
        uint8_t *&write = fh->m_write;

        while (read < write)
        {
            IO::outb(IO::COM1, *(read++));
        }

        read  = base;
        write = base;
    }
#endif


int libc_init_stdio()
{
    #ifdef __is_kernel
        auto *fh = kfilesystem::vfsFindFile("/dev/stdout");
        fh->stream.m_flush = stdout_flush;
        stdout = (FILE*)(fh);
    
    #else
        ksysc_request req;
        req.type = SYSC_FILE_GET;
        strcpy(req.msg, "/dev/stdout");
        libc_syscall(&req);
        stdout = (FILE*)(req.data);

    #endif

    return 1;
}

