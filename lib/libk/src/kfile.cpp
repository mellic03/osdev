#include <kfile.h>
// #include <string.h>

size_t
KFile_write( KFile *fh, const void *src, size_t nbytes )
{
    // klock_acquire(&fh->lock);

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

    // klock_release(&fh->lock);

    return count;
}


size_t
KFile_read( void *dst, KFile *fh, size_t nbytes )
{
    // klock_acquire(&fh->lock);

    uint8_t *d = (uint8_t*)dst;
    size_t count = 0;

    while ((count < nbytes) && (fh->read < fh->write))
    {
        *(d++) = (*fh->read++);
        count += 1;
    }

    // klock_release(&fh->lock);

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
