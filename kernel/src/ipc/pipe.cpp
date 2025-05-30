#include <ipc/pipe.hpp>
#include <filesystem/vfs.hpp>
#include <kernel/log.hpp>
#include <kmalloc.h>
#include <new>


static size_t pipe_read( vfsNode *fh, void *dst, size_t nbytes  )
{
    auto *p = (knl::Pipe*)(fh->m_impl);
    return p->read(dst, nbytes);
}

static size_t pipe_write( vfsNode *fh, const void *src, size_t nbytes )
{
    auto *p = (knl::Pipe*)(fh->m_impl);
    return p->write(src, nbytes);
}


vfsNode *knl::popen( const char *path, size_t stride )
{
    auto *fh = vfs::open(path);
    fh->m_flags  |= vfsFlag_ReadCstm | vfsFlag_WriteCstm;
    fh->m_readfn  = pipe_read;
    fh->m_writefn = pipe_write;
    fh->m_impl    = kmalloc(sizeof(knl::Pipe));
    auto *pipe = new (fh->m_impl) knl::Pipe();
          pipe->m_stride = stride;
    return (vfsNode*)fh;
}


int knl::pclose( const char * )
{
    return -1;
}

