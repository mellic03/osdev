#include <kassert.h>
#include <kpanic.h>
#include <kernel/log.hpp>
#include <kernel/kvideo.hpp>
#include "kernel/kvideo.hpp"
#include <cpu/cpu.hpp>


void kernel_assert( bool cond, const char *msg, const char *file,
                    const char *func, int line )
{
    if (!cond)
    {
        syslog::println(
            "assertion failed: (%s)\n\"%s\", line %d in function \"%s\"",
            msg, file, line, func
        );

        kvideo::fill(50, 75, 200, 255);
        CPU::hcf();
    }
}
