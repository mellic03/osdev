#include <kassert.h>
#include <kpanic.h>
#include <kernel/log.hpp>

void kernel_assert( const char *msg, bool cond,
                    const char *file, int line, const char *func )
{
    if (!cond)
    {
        syslog::printf(
            "[kassert failed] \"%s\", line %d in function \"%s\"",
            file, line, func
        );
        kpanic(msg);
    }
}
