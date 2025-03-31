#include <libc.h>
#include <stdio.h>
#include <string.h>
#include <ksysc.h>

void libc_init()
{
    libc_init_stdio();
}



// #define __is_kernel
#ifdef __is_kernel
    #include <kpanic.h>
    #include <kernel/log.hpp>
    #include <kernel/ioport.hpp>
    #include <kernel/vfs.hpp>

    static void stdout_flush( kfstream *fh )
    {
        size_t  &read  = fh->m_read;
        size_t  &write = fh->m_write;

        while (read != write)
        {
            IO::outb(IO::COM1, read);
            read = (read + 1) % fh->m_size;
        }

        read  = 0;
        write = 0;
    }
#endif


void libc_init_stdio()
{
    #ifdef __is_kernel
        syslog log("libc_init_stdio");
        auto *fh = vfsFindFile("/dev/stdout");
        log("fh: 0x%lx", fh);
        if (!fh) kpanic("Could not retrieve /dev/stdout");
        log("B");
        fh->stream.m_flush = stdout_flush;
        log("C");
        stdout = (FILE*)(fh);
        log("D");
    
    #else
        ksysc_request req;
        req.type = SYSC_FILE_GET;
        strcpy(req.msg, "/dev/stdout");
        libc_syscall(&req);
        stdout = (FILE*)(req.data);

    #endif
}

