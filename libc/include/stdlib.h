#pragma once

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


void *malloc( size_t nbytes );
void  free( void* );

char *itoa( int value, char *buf, int radix );
char *ltoa( int64_t value, char *buf, int radix );

char *utoa( uint32_t value, char *buf, int radix );
char *ultoa( uint64_t value, char *buf, int radix );



#ifdef __cplusplus
}
#endif

