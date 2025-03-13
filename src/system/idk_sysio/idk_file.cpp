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
        

size_t idk_file_write( idk_file_t *ptr, const void *src, size_t nbytes )
{
    idk_FILE *fh = static_cast<idk_FILE*>(ptr->impl);
    auto &rbuf = fh->buf;

    if (!rbuf.write((uint8_t*)src, nbytes))
    {
        return 0;
    }

    return nbytes;
}


int idk_file_flush( idk_file_t *ptr )
{
    idk_FILE *fh = static_cast<idk_FILE*>(ptr->impl);
    fh->status |= idk::FileStatus_DIRTY;
    return 0;
}



char *idk_file_data( idk_file_t *ptr )
{
    idk_FILE *fh = static_cast<idk_FILE*>(ptr->impl);
    return static_cast<char *>(fh->buf.data());
}


size_t idk_file_size( idk_file_t *ptr )
{
    idk_FILE *fh = static_cast<idk_FILE*>(ptr->impl);
    return fh->buf.capacity();
}


size_t idk_file_bytes_used( idk_file_t *ptr )
{
    idk_FILE *fh = static_cast<idk_FILE*>(ptr->impl);
    return fh->buf.bytes_used();
}


size_t idk_file_bytes_free( idk_file_t *ptr )
{
    idk_FILE *fh = static_cast<idk_FILE*>(ptr->impl);
    return fh->buf.bytes_free();
}

