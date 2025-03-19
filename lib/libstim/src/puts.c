#include <stim/stim.h>


void stim_putstr( const char *str )
{
    ksysc_io_request req = {
        .type  = SYSC_PUT_STRING,
        .flags = 0,
        .data = str
    };

    stim_syscall((ksysc_request*)(&req));
}
