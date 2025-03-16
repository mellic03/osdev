#include <driver/serial.hpp>

extern "C" int libc_stdio_init();

int libc_init()
{
    SYSLOG_BEGIN("libc_init");
    int ret0 = libc_stdio_init();
    
    SYSLOG_END();
    return 1;
}

