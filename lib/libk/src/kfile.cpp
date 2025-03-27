#include <kfile.h>
#include <string.h>
#include <kproc.hpp>


size_t
KFile_write( KFile *fh, const void *src, size_t nbytes )
{
    if (fh->write + nbytes < fh->eof)
    {
        memcpy(fh->write, src, nbytes);
        fh->write += nbytes;
        return nbytes;
    }


    const uint8_t *s = (const uint8_t*)src;
    size_t count = 0;

    while ((count < nbytes) && (fh->write < fh->eof))
    {
        *(fh->write++) = *(s++);
        count += 1;
    }

    if (fh->write >= fh->eof)
    {
        fh->fsh(fh);
    }

    return count;
}


size_t
KFile_read( void *dst, KFile *fh, size_t nbytes )
{
    kthread::yield();

    if (fh->read + nbytes < fh->write)
    {
        memcpy(dst, fh->read, nbytes);
        fh->read += nbytes;
        return nbytes;
    }

    uint8_t *d = (uint8_t*)dst;
    size_t count = 0;

    while ((count < nbytes) && (fh->read < fh->write))
    {
        *(d++) = (*fh->read++);
        count += 1;
    }

    return count;
}
// size_t
// KFile_write( KFile *fh, const uint8_t *src, size_t nbytes )
// {
//     if (fh->tail + nbytes > fh->eof)
//     {
//         return 0;
//     }

//     memcpy(fh->tail, src, nbytes);
//     fh->tail += nbytes;

//     return nbytes;
// }


// size_t
// KFile_read( uint8_t *dst, KFile *fh, size_t nbytes )
// {
//     size_t n = 0;

//     if (fh->read+nbytes >= fh->tail)
//     {
//         n = fh->tail-fh->read;
//         memcpy(dst, fh->read, n);
//         fh->read = fh->base;
//         fh->head = fh->base;
//     }

//     else
//     {
//         n = nbytes;
//         memcpy(dst, fh->read, nbytes);
//         fh->read += nbytes;
//     }

//     return n;
// }
