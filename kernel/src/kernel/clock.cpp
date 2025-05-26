#include <kernel/clock.hpp>
#include <cpu/cpu.hpp>

// static std::atomic_uint64_t uptime_usecs{0};


uint64_t kclock::now()
{
    auto *cpu = SMP::this_cpu();
    return cpu->m_msecs.load() / 1000;
    // return CPU::getTSC() / 5000;
}

// void kclock::detail::tick( uint64_t us )
// {
//     auto *cpu = SMP::this_cpu();
//     cpu->m_msecs.store(us);
// }
