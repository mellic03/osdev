#include <kernel.h>
#include <string.h>
#include <stdio.h>

#include "../log/log.hpp"
#include "./syscall.hpp"
#include "../interrupt/interrupt.hpp"
#include "../kfs/kfs.hpp"

using namespace idk;




static void
sysc_file_create( ksysc_request *req )
{
    syslog log("sysc_file_create");

    const char *path = (const char*)req->data;
    KFile *fh = KFS::KFile_create(1024, nullptr);

}



static void
file_get( ksysc_request *req )
{
    KFile *fh = nullptr;

    switch (req->flags)
    {
        default: break;
        case SYSF_FILE_KDEVKEY: fh = KFS::kdevkey;   break;
        case SYSF_FILE_KDEVSCN: fh = KFS::kdevscn;   break;
    }

    req->data = reinterpret_cast<void*>(fh);
}



static void
file_flush( ksysc_request *req )
{
    syslog log("file_flush");
    KFile *fh = reinterpret_cast<KFile*>(req->data);
    fh->fsh(fh);
    log("fh: 0x%lx", fh);
}



static void
stdio_get( ksysc_request *req )
{
    req->data = (void*)(&KFS::kstdio);
}




void
idk::syscall_handler( kstackframe *frame )
{
    syslog log("syscall_handler");
    auto *req = (ksysc_request*)(frame->rdi);
    log("type: %u", req->type);

    switch (req->type)
    {
        default: break;
        case SYSC_FILE_CREATE:   sysc_file_create(req); break;
        case SYSC_FILE_GET:      file_get(req);         break;
        case SYSC_FILE_FLUSH:    file_flush(req);       break;
        case SYSC_FILE_GETSTDIO: stdio_get(req);        break;
        case SYSC_MEM_ALLOC:     req->data = kmalloc(req->size); break;
        case SYSC_MEM_FREE:      kfree(req->data);
    }

    // SYSLOG_END();
}

