#include <kassert.h>
#include <kpanic.hpp>
// #include <kprintf.hpp>
// #include <kernel/log.hpp>
// #include <driver/video.hpp>


void kernel_assert( bool cond, const char *msg, const char *file,
                    const char *func, int line )
{
    if (!cond)
    {
        // syslog::println(
        //     "assertion failed: (%s)\n\"%s\", line %d in function \"%s\"",
        //     msg, file, line, func
        // );

        kpanic(
            "assertion failed: (%s)\n\"%s\", line %d in function \"%s\"",
            msg, file, line, func
        );

        // kvideo2::fill(u8vec4(50, 75, 200, 255));

        // while (true)
        // {
        //     asm volatile ("cli; hlt");
        // }
    }
}
