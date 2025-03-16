#include "syscall.hpp"
#include <stdio.h>
#include "system/drivers/serial.hpp"
#include "system/idk_sysio/sysio_syscall.hpp"
#include "system/system.hpp"
#include "log.hpp"
using namespace idk;


void internal::__syscall_handler() // ( uint64_t ecode )
{
    auto &req = *internal::__sysreq;

    SYSLOG_BEGIN("__syscall_handler");
    // SYSLOG("ecode=%u\n", ecode);
    SYSLOG("type=%u\n", req.type);

    SYSLOG_END
    // switch (req.type)
    // {
    //     using namespace idk::sysio;
    //     using enum idk::SysRequestType::____;

    //     case NONE:                                      break;
    //     case FILE_CREATE:   __syscall_file_create();    break;
    //     case FILE_DELETE:   __syscall_file_delete();    break;
    //     case FILE_RENAME:   __syscall_file_rename();    break;
    //     // case FILE_GETSTDIO: __syscall_file_getstdio();  break;
    // }

}