#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif




typedef struct
{
    uint32_t  fd;
    uint32_t  flags;
    uint8_t  *data;
    uint64_t  nbytes;
    uint64_t  cursor;
    uint32_t  status;

} __ck_win_ctx;






#ifdef __cplusplus
}
#endif

