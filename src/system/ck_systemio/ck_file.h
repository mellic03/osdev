#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct __ck_FILE
{
    uint32_t  fd;
    uint32_t  flags;
    uint8_t  *data;
    uint8_t  *end;
    uint8_t  *head;
    uint8_t  *tail;
    uint32_t  status;

} __attribute__((packed)) ckFILE;




typedef struct
{
    ckFILE  iofiles[4];
    uint8_t iobufs[4][512];
    uint8_t sysports[256];
} ckMappedIO;



/**
 * write nbytes of src into fh
 */
size_t  ckFILE_write( ckFILE *fh, const void *src, size_t nbytes );

/**
 * write nbytes of src into fh
 */
// size_t  ckFILE_read( void *dst, ckFILE*, size_t nbytes );

/**
 * write nbytes of src into fh
 */
// size_t  ckFILE_flush();


ckFILE *ckFILE_frombuf( void *buf, size_t size );


#ifdef __cplusplus
}
#endif

