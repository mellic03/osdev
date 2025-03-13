#include <stdio.h>


FILE *stderr = NULL;
FILE *stdin  = NULL;
FILE *stdout = NULL;


int __libc_stdio_init( FILE *std_err, FILE *std_in, FILE *std_out )
{
    stderr = std_err;
    stdin  = std_in;
    stdout = std_out;

    return 1;
}


int fflush( FILE *fh )
{
    idk_file_flush(fh);
    return 0;
}


int fputc( char c, FILE *fh )
{
    return idk_file_write(fh, &c, 1);
}



int fputs( const char *str, FILE *fh )
{
    while (*str)
    {
        fputc(*str, fh);
        str++;
    }

    return 1;
}


size_t fwrite( const void *buf, size_t size, size_t count, FILE *fh )
{
    auto *src = static_cast<const unsigned char*>(buf);

    if (idk_file_write(fh, buf, count*size) == count*size)
    {
        return count*size;
    }

    for (size_t i=0; i<size; i++)
    {
        fputc(src[i], fh);
    }

    return count;
};




int fseek( FILE *fh, long offset, int origin )
{
    // fh->status |= ck::FileStatus_DIRTY;
    // uint8_t *backup = fh->tail;

    // switch (origin)
    // {
    //     default: return 1;
    
    //     case SEEK_SET: fh->tail  = fh->data+offset;  break;
    //     case SEEK_CUR: fh->tail += offset;           break;
    //     case SEEK_END: fh->tail  = fh->end - offset; break;
    // }

    // if (fh->tail < fh->data || fh->end <= fh->tail)
    // {
    //     fh->tail = backup;
    //     return 1;
    // }

    return 0;
}




int putc( char c ) { return fputc(c, stdout); }
int puts( const char *str ) { return fputs(str, stdout); }

