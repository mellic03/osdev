#pragma once
#include <kdef.h>
#include <idk_fptr.hpp>

struct vfsNode;

namespace knl
{
    struct MsState;
    struct MsEvent;
    struct KbEvent;
}

namespace usrknl
{
    inline void   (*panic)(const char*);
    inline void   (*hcf)();

    inline vfsNode* (*popen)(const char*, size_t stride);
    inline void*  (*fopen)(const char*);
    inline size_t (*fread)(void *fh, void *dst, size_t nbytes);
    inline size_t (*fwrite)(void *fh, const void *src, size_t nbytes);

    inline ModuleInterface *(*findModule)(uint64_t, uint64_t);

}

