#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ck/ck_file.h>
#include <ck_systemio/ck_systemio.h>
#include <ck_systemio/ck_file.hpp>
#include <system/io.hpp>
#include <system/drivers/serial.hpp>


FILE *stderr = NULL;
FILE *stdin  = NULL;
FILE *stdout = NULL;
FILE *tmpfh  = NULL;

static void __internal_fprintf( FILE*, const char *&, va_list& );
// static char __ck_libc_fprintf_buf[32];
// static FILE *__ck_libc_sprintf_fh;


void flush_stdout()
{
    uint8_t *start = stdout->head;
    uint8_t *end   = stdout->tail;

    for (uint8_t *str=start; str<end; str++)
    {
        ck::IO::outb(ck::serial::COM1, *str);
    }

    stdout->head = (stdout->data + 0);
    stdout->tail = (stdout->data + 0);
    stdout->status &= !ck::FileStatus_DIRTY;
}




int __libc_stdio_init( void *ptr )
{
    ckMappedIO *map = (ckMappedIO*)(ptr);

    stderr = &(map->iofiles[0]);
    stdin  = &(map->iofiles[1]);
    stdout = &(map->iofiles[2]);
    tmpfh  = &(map->iofiles[3]);

    for (uint8_t *ptr=stdout->data; ptr<stdout->end; ptr++)
    {
        *ptr = '\0';
    }

    return 1;
}



int fseek( FILE *fh, long offset, int origin )
{
    fh->status |= ck::FileStatus_DIRTY;
    uint8_t *backup = fh->tail;

    switch (origin)
    {
        default: return 1;
    
        case SEEK_SET: fh->tail  = fh->data+offset;  break;
        case SEEK_CUR: fh->tail += offset;           break;
        case SEEK_END: fh->tail  = fh->end - offset; break;
    }

    if (fh->tail < fh->data || fh->end <= fh->tail)
    {
        fh->tail = backup;
        return 1;
    }

    return 0;
}



int fputc( char c, FILE *fh )
{
    return ckFILE_write(fh, &c, 1);
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



int fprintf( FILE *fh, const char *fmt, ... )
{
    va_list args;
    va_start(args, fmt);

    while (*fmt)
    {
        if (*fmt == '%')
        {
            __internal_fprintf(fh, fmt, args);
        }

        else
        {
            fputc(*fmt, fh);
        }

        fmt += 1;
    }

    va_end( args );

    flush_stdout();
    memset(fh->data, '\0', (fh->tail - fh->data));
    fh->tail = fh->data;

    return 1;
}



int sprintf( char *buf, const char *fmt, ... )
{
    FILE *fh = ckFILE_frombuf(buf, sizeof(fmt));

    // fh->fd     = 0;
    // fh->flags  = 0;
    // fh->data   = (uint8_t*)buf;
    // fh->end    = fh->data + strlen(fmt);
    // fh->tail = 0;
    // fh->status = 0;

    va_list args;
    va_start(args, fmt);
    int n = fprintf(fh, fmt, args);
    va_end(args);

    return n;
}





static void __internal_fprintf( FILE *fh, const char *&fmt, va_list &args )
{
    char buf[32]; // = __ck_libc_fprintf_buf;
    int idx = 0;
    int lng = 0;

    fmt++;

    switch (*fmt)
    {
        default: break;

        case 'l': lng = 1; break;

        case 'c': fputc(va_arg(args, int), fh);                break;
        case 's': fputs(va_arg(args, const char *), fh);       break;

        case 'd': idx = itoa(va_arg(args,      int), buf, 10); break;
        case 'u': idx = utoa(va_arg(args, uint32_t), buf, 10); break;
        case 'x': idx = utoa(va_arg(args, uint32_t), buf, 16); break;
    }

    if (lng)
    {
        fmt++;

        switch (*fmt)
        {
            default: break;
            case 'd': idx = ltoa(va_arg(args,  int64_t), buf, 10);  break;
            case 'u': idx = ultoa(va_arg(args, uint64_t), buf, 10); break;
            case 'x': idx = ultoa(va_arg(args, uint64_t), buf, 16); break;
        }
    }

    else if (idx)
    {
        buf[idx] = '\0';
        fputs(buf, fh);
    }

}
