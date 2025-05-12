// #include <atomic>
// #include <kernel/interrupt.hpp>
// #include <kassert.h>
// #include <driver/pic.hpp>

// using lock_t = volatile int;

// #define acquireLock(lock)                                                                                              \
//     ({                                                                                                                 \
//         unsigned i = 0;                                                                                                \
//         while (__atomic_exchange_n(lock, 1, __ATOMIC_ACQUIRE) && ++i < 0x2FFFFFFF)                                     \
//             asm("pause");                                                                                              \
//         if (i >= 0x2FFFFFFF) {                                                                                         \
//             kassert(!"Deadlock!");                                                                                      \
//         }                                                                                                              \
//     })

// #define acquireLockIntDisable(lock)                                                                                    \
//     ({                                                                                                                 \
//         unsigned i = 0;                                                                                                \
//         kassert(checkInterrupts());                                                                                     \
//         asm volatile("cli");                                                                                           \
//         while (__atomic_exchange_n(lock, 1, __ATOMIC_ACQUIRE) && ++i < 0x2FFFFFFF)                                     \
//             asm volatile("sti; pause; cli");                                                                           \
//         if (i >= 0x2FFFFFFF) {                                                                                         \
//             kassert(!"Deadlock!");                                                                                      \
//         }                                                                                                              \
//     })

// #define releaseLock(lock) ({ __atomic_store_n(lock, 0, __ATOMIC_RELEASE); });

// #define acquireTestLock(lock)                                                                                          \
//     ({                                                                                                                 \
//         int status;                                                                                                    \
//         status = __atomic_exchange_n(lock, 1, __ATOMIC_ACQUIRE);                                                       \
//         status;                                                                                                        \
//     })



// struct kspinlock
// {
//     lock_t m_lock;

//     void lock() noexcept
//     {
//         acquireLockIntDisable(&m_lock);
//     }

//     void unlock() noexcept
//     {
//         releaseLock(&m_lock);
//     }
// };

    

// // struct kspinlock
// // {
// //     std::atomic_bool m_lock{0};

// //     void lock() noexcept
// //     {
// //         asm volatile ("cli");

// //         while (true)
// //         {
// //             // Optimistically assume the lock is free on the first try
// //             if (!m_lock.exchange(true, __ATOMIC_ACQUIRE))
// //             {
// //                 return;
// //             }
// //             // Wait for lock to be released without generating cache misses
// //             while (m_lock.load(__ATOMIC_RELAXED))
// //             {
// //                 // Issue X86 PAUSE or ARM YIELD instruction to reduce contention between
// //                 // hyper-threads
// //                 __builtin_ia32_pause();
// //             }
// //         }
// //     }

// //     bool try_lock() noexcept
// //     {
// //         // First do a relaxed load to check if lock is free in order to prevent
// //         // unnecessary cache misses if someone does while(!try_lock())
// //         return !m_lock.load(__ATOMIC_RELAXED) && !m_lock.exchange(true, __ATOMIC_RELAXED);
// //     }

// //     void unlock() noexcept
// //     {
// //         m_lock.store(false, __ATOMIC_RELEASE);
// //         asm volatile ("sti");
// //     }
// // };
    
    
        