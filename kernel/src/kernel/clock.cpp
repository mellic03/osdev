#include <kernel/clock.hpp>

static std::atomic_uint64_t uptime_msecs{0};


uint64_t kclock::now()
{
    return uptime_msecs.load();
}

void kclock::detail::tick( uint64_t ms )
{
    uptime_msecs += ms;
}
