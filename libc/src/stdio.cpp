#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include <idk_file.h>
#include <idk_syscall.h> 
#include <idk_virtio.h> 


FILE *stderr = NULL;
FILE *stdin  = NULL;
FILE *stdout = NULL;


extern "C"
int libc_stdio_init( FILE *std_err, FILE *std_in, FILE *std_out )
{
    outvirt8(KFILE_READ, KTRGT_STDERR);
    stderr = (FILE*)(invirt64(KFILE_READ));

    outvirt8(KFILE_READ, KTRGT_STDIN);
    stdin = (FILE*)(invirt64(KFILE_READ));

    outvirt8(KFILE_READ, KTRGT_STDOUT);
    stdout = (FILE*)(invirt64(KFILE_READ));

    return 1;
}

// int __libc_stdio_init()
// {
//     serial_printf("[__libc_stdio_init]\n");
//     idk::SysRequest req(idk::SysRequestType::FILE_GETSTDIO);

//     auto &res  = idk::syscall(req);
//     auto &fres = *(idk::SysResponse_FILE*)(&res);

//     idk_FILE *files = (idk_FILE*)(fres.addr);
//     stderr = (idk_file_t*)(files+0);
//     stdin  = (idk_file_t*)(files+1);
//     stdout = (idk_file_t*)(files+2);

//     return 1;
// }


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

