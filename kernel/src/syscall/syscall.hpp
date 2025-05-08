#pragma once
#include <kernel/syscall.h>
#include <kernel/interrupt.hpp>

namespace kernel
{
    void syscallISR( intframe_t* );
}
