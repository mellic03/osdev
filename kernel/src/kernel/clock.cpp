#include <kernel/clock.hpp>

static std::atomic_uint64_t uptime_usecs{0};


uint64_t kclock::now()
{
    return uptime_usecs.load() / 1000;
}

void kclock::detail::tick( uint64_t microseconds )
{
    uptime_usecs += microseconds;
}
