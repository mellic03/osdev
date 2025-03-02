#include "ck_file.hpp"
#include <cstring.hpp>




size_t ckFILE_write( ckFILE *fh, const void *src, size_t nbytes )
{
    fh->status |= ck::FileStatus_DIRTY;

    if (fh->tail + nbytes >= fh->end)
    {
        return 0;
    }

    std::memcpy(fh->tail, src, nbytes);
    fh->tail += nbytes;

    return nbytes;
}



// size_t ckFILE_read( void *dst, ckFILE *fh, size_t nbytes )
// {

// }

// size_t ckFILE_flush()
// {

// }


ckFILE *ckFILE_frombuf( void *buf, size_t size )
{
    static ckFILE fh;
    uint8_t *base = (uint8_t*)(buf);

    fh = {
        ck::FILE_MAGIC, ck::FileFlag_BUF,
        base, base+size,
        base, base,
        ck::FileStatus_GOOD
    };

    return &fh;
}

