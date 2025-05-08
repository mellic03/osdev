#include <atomic>
#include <driver/pic.hpp>


struct kspinlock
{
    std::atomic_bool m_lock{0};

    void lock() noexcept
    {
        asm volatile ("cli");

        while (true)
        {
            // Optimistically assume the lock is free on the first try
            if (!m_lock.exchange(true, __ATOMIC_ACQUIRE))
            {
                return;
            }
            // Wait for lock to be released without generating cache misses
            while (m_lock.load(__ATOMIC_RELAXED))
            {
                // Issue X86 PAUSE or ARM YIELD instruction to reduce contention between
                // hyper-threads
                __builtin_ia32_pause();
            }
        }
    }

    bool try_lock() noexcept
    {
        // First do a relaxed load to check if lock is free in order to prevent
        // unnecessary cache misses if someone does while(!try_lock())
        return !m_lock.load(__ATOMIC_RELAXED) && !m_lock.exchange(true, __ATOMIC_RELAXED);
    }

    void unlock() noexcept
    {
        m_lock.store(false, __ATOMIC_RELEASE);
        asm volatile ("sti");
    }
};

