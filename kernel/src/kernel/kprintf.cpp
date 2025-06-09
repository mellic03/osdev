#include <kprintf.hpp>
#include <kernel/tty.hpp>
#include <stdio.h>

static knl::kTTY kprintf_tty;
static char kprintf_buf[512];

void kprintf( const char *fmt, ... )
{
    va_list vlist;
    va_start(vlist, fmt);
    kvprintf(fmt, vlist);
    va_end(vlist);
    // vfs::write(fh_stdout, buf, 0, (size_t)n);
    // textright->putstr(buf);
}


void kvprintf( const char *fmt, va_list vlist )
{
    vsnprintf(kprintf_buf, sizeof(kprintf_buf), fmt, vlist);
    kprintf_buf[sizeof(kprintf_buf) - 1] = '\0';
    kprintf_tty.putstr(kprintf_buf);
}

