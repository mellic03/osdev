#include "syscall.hpp"
#include <stdio.h>
#include "system/drivers/serial.hpp"
#include "system/idk_sysio/sysio_syscall.hpp"

using namespace idk;


void internal::__syscall_handler()
{
    auto &req = *internal::__sysreq;
    serial_printf("[__syscall_handler] SYSCALL, type=%d\n", req.type);

    switch (req.type)
    {
        using namespace idk::sysio;
        using enum idk::SysRequestType::____;

        case NONE:          break;
        case FILE_CREATE:   __syscall_file_create(); break;
        case FILE_DELETE:   __syscall_file_delete(); break;
        case FILE_RENAME:   __syscall_file_rename(); break;

    }

}