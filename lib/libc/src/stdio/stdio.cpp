#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

FILE *stderr = nullptr;
FILE *stdin  = nullptr;
FILE *stdout = nullptr;


#ifdef __is_kernel
    #include <kthread.hpp>
#endif


int fflush( FILE* )
{
    return 0;
}



char *fgets( char *str, int n, FILE *fh )
{
    if (str || n || fh)
    {
        
    }
    // int i=0;

    // while (i<n && (fh->read < fh->write))
    // {
    //     i++;
    //     *(str++) = *(fh->read++);
    // }

    // if (i==0)
    // {
    //     return nullptr;
    // }

    return str;
}


int fputc( char, FILE* )
{
    return 0;
    // return idk_file_write(fh, &c, 1);
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


// size_t fwrite( const void *buf, size_t size, size_t count, FILE *fh )
// {
//     auto *src = static_cast<const unsigned char*>(buf);

//     if (idk_file_write(fh, buf, count*size) == count*size)
//     {
//         return count*size;
//     }

//     for (size_t i=0; i<size; i++)
//     {
//         fputc(src[i], fh);
//     }

//     return count;
// };




// int fseek( FILE *fh, long offset, int origin )
// {
//     // fh->status |= ck::FileStatus_DIRTY;
//     // uint8_t *backup = fh->tail;

//     // switch (origin)
//     // {
//     //     default: return 1;
    
//     //     case SEEK_SET: fh->tail  = fh->data+offset;  break;
//     //     case SEEK_CUR: fh->tail += offset;           break;
//     //     case SEEK_END: fh->tail  = fh->end - offset; break;
//     // }

//     // if (fh->tail < fh->data || fh->end <= fh->tail)
//     // {
//     //     fh->tail = backup;
//     //     return 1;
//     // }

//     return 0;
// }


// int putc( char c ) { return fputc(c, stdout); }
// int puts( const char *str ) { return fputs(str, stdout); }

