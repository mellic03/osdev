#include <kprintf.hpp>
#include <sys/tty0.hpp>
#include <stdio.h>

static char kprintf_buf[512];

void kprintf( const char *fmt, ... )
{
    va_list vlist;
    va_start(vlist, fmt);
    kvprintf(fmt, vlist);
    va_end(vlist);
}


void kvprintf( const char *fmt, va_list vlist )
{
    vsnprintf(kprintf_buf, sizeof(kprintf_buf), fmt, vlist);
    kprintf_buf[sizeof(kprintf_buf) - 1] = '\0';

    knl::tty0_stdout.emit((const char*)kprintf_buf);
    // knl::tty0.putstr(kprintf_buf);
}

