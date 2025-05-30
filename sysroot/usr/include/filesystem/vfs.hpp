#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <mutex>

enum vfsEntry_: uint32_t
{
    vfsNode_Invalid = 0,
    vfsNode_Directory,
    vfsNode_File,
};


enum vfsFlag_: uint32_t
{
    vfsFlag_None      = 0,
    vfsFlag_Symlink   = 1<<0,
    vfsFlag_ReadCstm  = 1<<8,
    vfsFlag_WriteCstm = 1<<9,
};


struct vfsInode
{
    uint64_t  idx;
    int64_t  next;
    int64_t  block;
    uint64_t size;
    vfsInode()
    : idx(0), next(-1), block(-1), size(0) {  };
};


struct vfsNode
{
    static constexpr size_t NAME_MAXLEN = 32;

    uint32_t m_type;
    uint32_t m_flags;
    void     *m_impl;
    size_t  (*m_readfn)(vfsNode*, void*, size_t);
    size_t  (*m_writefn)(vfsNode*, const void*, size_t);
    char     m_name[NAME_MAXLEN+1];
    int64_t  m_inode;

    vfsNode( const char *name, uint32_t type );
};


struct vfsDirListing
{
    size_t  count;
    vfsNode entries[];
};



enum vfsSeek_: uint8_t
{
    vfsSeek_Invalid = 0,
    vfsSeek_Set,
    vfsSeek_Cur,
    vfsSeek_End,
};


namespace vfs
{
    static constexpr size_t BLOCK_SIZE = 4096;

    void     init();
    void     print();
    void     walk( bool (*cond)(vfsNode*, int depth) );

    vfsNode *open ( const char *path );
    size_t   write( vfsNode *fh, const void *src, size_t offset, size_t nbytes );
    size_t   read ( vfsNode *fh, void *dst, size_t offset, size_t nbytes );
    size_t   tell ( vfsNode *fh );
    int      seek ( vfsNode *fh, int64_t offset, uint8_t mode );
    size_t   size ( vfsNode *fh );

}



struct vfsBlock
{
    uint8_t data[vfs::BLOCK_SIZE];
};

