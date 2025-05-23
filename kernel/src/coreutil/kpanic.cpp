#include <kpanic.h>
#include <kernel/log.hpp>
#include <kernel/kvideo.hpp>
#include <cpu/cpu.hpp>


void kpanic( const char *msg )
{
    syslog log("KERNEL PANIC");
    log("reason: %s", msg);

    kvideo::fillColor(50, 75, 200, 255);
    kvideo::fillBuffer(kvideo::frontbuffer);
    CPU::hcf();
}

