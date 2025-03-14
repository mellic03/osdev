#include "./sysio_syscall.hpp"
#include "system/syscall/syscall.hpp"
#include <stdio.h>

using namespace idk;


void
idk::sysio::__syscall_file_create()
{
    auto &req = *idk::internal::__sysreq;
    auto &res = *((SysFileResponse*)idk::internal::__sysres);

    req.buf.back() = '\0';
    const char *filename = &(req.buf[0]);

    if (strlen(filename) == req.buf.capacity() - 1)
    {
        // Invalid buffer
        res.type = SysResponse_FAILURE;
        sprintf(res.msg, "Invalid buffer");
        return;
    }

    res.type = SysResponse_SUCCESS;
    sprintf(res.msg, "Created file: \"%s\"", filename);
}
    

void
idk::sysio::__syscall_file_delete()
{
    // auto &req = *idk::internal::__sysreq;
    auto &res = *((SysFileResponse*)idk::internal::__sysres);

    res.type = SysResponse_FAILURE;
    sprintf(res.msg, "Not implemented");
}

    
void
idk::sysio::__syscall_file_rename()
{
    // auto &req = *idk::internal::__sysreq;
    auto &res = *((SysFileResponse*)idk::internal::__sysres);

    res.type = SysResponse_FAILURE;
    sprintf(res.msg, "Not implemented");
}

