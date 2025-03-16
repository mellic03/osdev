#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// typedef void* idk_file_t;

typedef struct
{
    uint64_t dummy;
} idk_file_t;


/** return size of fh in bytes */
size_t idk_file_size( idk_file_t *fh );

/** return size of written region in bytes */
size_t idk_file_bytes_used( idk_file_t *fh );

/** return size of unwritten region in bytes */
size_t idk_file_bytes_free( idk_file_t *fh );

/** write nbytes of src into fh */
size_t idk_file_write( idk_file_t *fh, const void *src, size_t nbytes );

/** return raw pointer to file data */
char *idk_file_data( idk_file_t *fh );

int idk_file_flush( idk_file_t *fh );



#ifdef __cplusplus
}
#endif

