#include <stdio.h>


size_t fwrite( const void *buf, size_t size, size_t count, FILE *fh )
{
    auto *src = static_cast<const unsigned char*>(buf);

    if (ckFILE_write(fh, buf, count*size) == count*size)
    {
        return count*size;
    }

    for (size_t i=0; i<size; i++)
    {
        fputc(src[i], fh);
    }

    return count;
    // fputc()
};




int putc( char c )
{
    return fputc(c, stdout);
}


int puts( const char *str )
{
    return fputs(str, stdout);
}


int printf( const char *fmt, ... )
{
    va_list args;
    va_start (args, fmt);
    int n = fprintf(stdout, fmt, args);
    va_end(args);
    return n;
}
