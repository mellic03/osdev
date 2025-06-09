#include <kpanic.hpp>
#include <kprintf.hpp>
#include <kernel/log.hpp>
#include <driver/video.hpp>


void kpanic( const char *fmt, ... )
{
    va_list vlist;
    va_start(vlist, fmt);
        syslog::print("[kpanic] ");
        syslog::vprint(fmt, vlist);
        syslog::print("\n");

        kprintf("[kpanic]");
        kvprintf(fmt, vlist);
        kprintf("\n");
    va_end(vlist);

    kvideo2::fill(u8vec4(50, 75, 200, 255));
    kvideo2::flush();

    while (true)
    {
        asm volatile ("cli; hlt");
    }
}


