#pragma once

#include <stddef.h>
#include <stdint.h>


namespace std
{
    int itoa( int value, char *sp, int radix );
    int utoa( uint32_t value, char *sp, int radix );

    // int ltoa( int64_t value, char *sp, int radix );
    // int ultoa( uint64_t value, char *sp, int radix );

}
