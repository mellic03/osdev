#pragma once
#include <kernel/syscall.h>
#include <kernel/interrupt.hpp>

namespace knl
{
    void syscallISR( intframe_t* );
}
