#include <kpanic.h>
#include <kernel/log.hpp>
#include "kernel/video.hpp"

void kpanic( const char *msg )
{
    syslog log("KERNEL PANIC");
    log("reason: %s", msg);

    kvideo::fill(50, 75, 200, 255);

    while (true)
    {
        asm volatile ("cli; hlt");
    }
}

