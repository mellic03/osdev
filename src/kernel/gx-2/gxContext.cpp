#include "gxContext.hpp"
#include <kmalloc.h>
#include <new>
#include <mutex>

static gx_Context  gx_stack[16];
static gx_Context *gx_stop  = gx_stack + 0;
static gx_Context  gx_current;
static std::mutex  gx_ctxlock;

#define CTX_LOCK(Content)\
    gx_ctxlock.lock();\
    Content\
    gx_ctxlock.unlock();\


void gxMakeCurrent( gx_Context *ctx )
{
    CTX_LOCK(gx_current = *ctx;)
}

gx_Context *gxGetCurrent()
{
    gx_Context *ctx = nullptr;
    CTX_LOCK(ctx = &gx_current;)
    return ctx;
}

void gxPushContext()
{
    CTX_LOCK(*(gx_stop++) = gx_current;)
}

void gxPopContext()
{
    CTX_LOCK(gx_current = *(--gx_stop);)
}
