#pragma once
#include <stdint.h>


#ifdef __cplusplus

    template <uint8_t vcode>
    void KInterrupt()
    {
        asm volatile(
            "int %0"
            :
            : "i" (vcode)
        );
    }

#endif


