#include <kassert.h>
#include <kpanic.h>
#include <kernel/log.hpp>
#include <kernel/kvideo.hpp>
#include "kernel/kvideo.hpp"
#include <khang.h>


void kernel_assert( const char *msg, bool cond, const char *file, int line, const char *func )
{
    if (!cond)
    {
        syslog::println(
            "assertion failed: (%s)\n\"%s\", line %d in function \"%s\"",
            msg, file, line, func
        );

        kvideo::fillColor(50, 75, 200, 255);
        kvideo::fillBuffer(kvideo::frontbuffer);
        kernel::hang();
    }
}
