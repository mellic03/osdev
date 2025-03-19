#include "./idk_file.h"
#include "./idk_file.hpp"
#include <cstring.hpp>



idk_FILE::idk_FILE( uint8_t *start, uint8_t *end, uint32_t fflags )
:   buf(start, end)
{
    fd = 0;
    flags = fflags;
    status = idk::FileStatus_GOOD;
}
        

size_t idk_file_write( void *ptr, const void *src, size_t nbytes )
{
    idk_FILE *fh = (idk_FILE*)ptr;
    auto &rbuf = fh->buf;

    if (!rbuf.write((uint8_t*)src, nbytes))
    {
        return 0;
    }

    return nbytes;
}


int idk_file_flush( void *ptr )
{
    idk_FILE *fh = (idk_FILE*)ptr;
    fh->status |= idk::FileStatus_DIRTY;
    return 0;
}


char *idk_file_data( void *ptr )
{
    idk_FILE *fh = (idk_FILE*)ptr;
    return static_cast<char *>(fh->buf.data());
}


size_t idk_file_size( void *ptr )
{
    idk_FILE *fh = (idk_FILE*)ptr;
    return fh->buf.capacity();
}


size_t idk_file_bytes_used( void *ptr )
{
    idk_FILE *fh = (idk_FILE*)ptr;
    return fh->buf.bytes_used();
}


size_t idk_file_bytes_free( void *ptr )
{
    idk_FILE *fh = (idk_FILE*)ptr;
    return fh->buf.bytes_free();
}

