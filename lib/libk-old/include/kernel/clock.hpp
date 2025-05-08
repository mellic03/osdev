#pragma once
#include <stddef.h>
#include <stdint.h>
#include <atomic>

namespace kclock
{
    extern std::atomic_uint64_t uptime_msecs; // = 0;
};

