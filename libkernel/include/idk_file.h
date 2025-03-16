#pragma once
#include <AAA.h>

#define IDK_FILE_MAGIC = 0xDEADBEBE;

enum idk_FileFlag: uint32_t
{
    idk_FFLAG_BUF    = 1 << 1,
    idk_FFLAG_MMAP   = 1 << 2,
    idk_FFLAG_STR    = 1 << 3,
    idk_FFLAG_OWNING = 1 << 4,

    idk_FFLAG_STDIO  = 1 << 16,
    idk_FFLAG_STDIN  = 1 << 17 | idk_FFLAG_STDIO | idk_FFLAG_MMAP,
    idk_FFLAG_STDOUT = 1 << 18 | idk_FFLAG_STDIO | idk_FFLAG_MMAP,
    idk_FFLAG_STDERR = 1 << 19 | idk_FFLAG_STDIO | idk_FFLAG_MMAP,
};

enum idk_FileStatus: uint32_t
{
    idk_FSTAT_GOOD = 0,
    idk_FSTAT_DIRTY,
    idk_FSTAT_INVALID,
    idk_FSTAT_OTHER,
};


/** return size of fh in bytes */
size_t idk_file_size( void *fh );

/** return size of written region in bytes */
size_t idk_file_bytes_used( void *fh );

/** return size of unwritten region in bytes */
size_t idk_file_bytes_free( void *fh );

/** write nbytes of src into fh */
size_t idk_file_write( void *fh, const void *src, size_t nbytes );

/** return raw pointer to file data */
char *idk_file_data( void *fh );

int idk_file_flush( void *fh );



#include <AAB.h>
