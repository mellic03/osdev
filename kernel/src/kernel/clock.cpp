#include <kernel/clock.hpp>
#include <cpu/cpu.hpp>

// static std::atomic_uint64_t uptime_usecs{0};


uint64_t kclock::now()
{
    auto *cpu = SMP::this_cpu();
    return cpu->m_usecs.load() / 1000;
}

// void kclock::detail::tick( uint64_t us )
// {
//     auto *cpu = SMP::this_cpu();
//     cpu->m_usecs.store(us);
// }
