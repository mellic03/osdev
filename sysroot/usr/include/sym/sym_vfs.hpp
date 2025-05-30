#pragma once
#include <kdef.h>
#include <idk_fptr.hpp>

struct vfsNode;

namespace uvfs
{
    inline vfsNode *(*open)(const char*);
    inline size_t   (*read)(vfsNode *fh, void *dst, size_t offset, size_t nbytes);
    inline size_t   (*write)(vfsNode *fh, const void *src, size_t offset, size_t nbytes);
    inline size_t   (*tell)(vfsNode *fh);
    inline int      (*seek)(vfsNode*, int64_t offset, uint8_t mode );

}

