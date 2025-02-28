#pragma once
#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif



int itoa( int value, char *sp, int radix );
int utoa( uint32_t value, char *sp, int radix );



#ifdef __cplusplus
}
#endif

