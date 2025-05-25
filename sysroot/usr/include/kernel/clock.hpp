#pragma once
#include <stddef.h>
#include <stdint.h>
#include <atomic>

namespace kclock
{
    /**
     * @return total uptime in milliseconds.
     */
    uint64_t now();

    // namespace detail
    // {
    //     void tick( uint64_t microseconds );
    // }
};

