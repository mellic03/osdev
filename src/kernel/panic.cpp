#include "panic.hpp"
#include <kernel/log.hpp>
#include <kernel.h>
#include "kvideo/kvideo.hpp"
#include <kstackframe.h>


void kpanic_handler( kstackframe *frame )
{
    kvideo::fill(vec3(0.8, 0.2, 0.2));
    kvideo::swapBuffers();

    const char *msg = (const char*)(frame->rdi);
    syslog log("KERNEL PANIC");
    log("reason: %s", msg);

    kernel_hcf();
}
