#include <kernel.h>
#include <kdriver/serial.hpp>
#include <string.h>

#include "./syscall.hpp"
#include "../interrupt/interrupt.hpp"

using namespace idk;

// static uint32_t sc_type;
// static uint32_t sc_flags;
// static char     sc_buf[256];


// kret_t
// idk::syscall( uint32_t *type, uint32_t *flags, char buf[256] )
// {
//     sc_type  = *type;
//     sc_flags = *flags;
//     memcpy(sc_buf, buf, sizeof(sc_buf));

//     idk::Interrupt<INT_SYSCALL>();

//     return KRET_SUCCESS;
// }





static void
file_create()
{
    SYSLOG_BEGIN("file_create");

    

    SYSLOG_END();
}


static void
put_string( ksysc_file_request *req )
{
    char *str = (char*)(req->data);
    serialf("%s", str);
}



void
idk::syscall_handler( kstackframe *frame )
{
    // SYSLOG_BEGIN("syscall_handler");
    auto *req = (ksysc_request*)(frame->rax);

    // SYSLOG("type:  %u", frame->rax);
    // SYSLOG("data:  %u", frame->rbx);

    switch (req->type)
    {
        default: break;
        case SYSC_PUT_STRING: put_string((ksysc_file_request*)req); break;
    }

    // SYSLOG_END();
}



