#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kernel.h>
#include <string.h>
#include <stdio.h>

#include <kernel/log.hpp>
#include "./syscall.hpp"
#include "../interrupt/interrupt.hpp"
#include <kernel/vfs.hpp>

using namespace idk;




static void
sysc_file_create( ksysc_request* )
{
    // syslog log("sysc_file_create");

    // const char *path = (const char*)req->data;
    // KFile *fh = KFS::KFile_create(1024, nullptr);

}


static void
file_get( ksysc_request *req )
{
    req->data = kfilesystem::vfsFindFile(req->msg);
}



static void
file_flush( ksysc_request *req )
{
    syslog log("file_flush");
    KFile *fh = reinterpret_cast<KFile*>(req->data);
    fh->fsh(fh);
    log("fh: 0x%lx", fh);
}



#include "../sde/sde.hpp"

static void
sysc_window_create( ksysc_request *req )
{
    static ksysc_window_response res;
    auto *ctx = sde::createContext(ivec2(50), ivec2(200));
    ctx->m_style.border = vec4(0.5, 1.0, 0.5, 0.85);

    res = {
        .sde_ctx = ctx,
        .x       = &ctx->m_lx,
        .y       = &ctx->m_ly,
        .border  = &ctx->m_style.color[0]
    };

    req->res = &res;
}

static void
sysc_window_delete( ksysc_request *req )
{
    sde::destroyContext((sde::WindowContext*)(req->data));
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
        case SYSC_FILE_CREATE:   sysc_file_create(req);   break;
        case SYSC_FILE_GET:      file_get(req);           break;
        case SYSC_FILE_FLUSH:    file_flush(req);         break;
        case SYSC_WINDOW_CREATE: sysc_window_create(req); break;
        case SYSC_WINDOW_DELETE: sysc_window_delete(req); break;
        case SYSC_MEM_ALLOC:     req->data = kmalloc(req->size); break;
        case SYSC_MEM_FREE:      kfree(req->data);
    }

    // SYSLOG_END();
}

