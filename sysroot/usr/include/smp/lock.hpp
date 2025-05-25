#pragma once
#include <atomic>


namespace knl
{
    template <bool scoped=false>
    struct SpinLock
    {
        std::atomic_int  m_scope{0};
        std::atomic_bool m_lock{0};

        void lock() noexcept
        {
            while (true)
            {
                // Optimistically assume the lock is free on the first try
                if (!m_lock.exchange(true, std::memory_order_acquire))
                {
                    return;
                }
                // Wait for lock to be released without generating cache misses
                while (m_lock.load(std::memory_order_relaxed))
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
            return !m_lock.load(std::memory_order_relaxed) && !m_lock.exchange(true, std::memory_order_relaxed);
        }

        void unlock() noexcept
        {
            m_lock.store(false, std::memory_order_release);
        }
    };


    struct DummyLock
    {
        void lock() {  };
        void unlock() {  };
    };

    struct AtomicFlag
    {
    private:
        std::atomic_bool m_locked;

    public:
        AtomicFlag() { m_locked.store(false); }
        void set() { m_locked.store(true); }
        void clear() { m_locked.store(false); }
        bool test() { return m_locked.load(); }

        bool isset()   { return m_locked.load(); }
        bool isclear() { return !m_locked.load(); }

        void lock() { set(); };
        void unlock() { clear(); };
    };
}

