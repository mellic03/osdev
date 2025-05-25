#pragma once
#include <atomic>

// The volatile storage class was originally meant for memory-mapped I/O
// registers.  Within the kernel, register accesses, too, should be protected
// by locks, but one also does not want the compiler "optimizing" register
// accesses within a critical section.  But, within the kernel, I/O memory
// accesses are always done through accessor functions; accessing I/O memory
// directly through pointers is frowned upon and does not work on all
// architectures.  Those accessors are written to prevent unwanted
// optimization, so, once again, volatile is unnecessary.

// Another situation where one might be tempted to use volatile is
// when the processor is busy-waiting on the value of a variable.  The right
// way to perform a busy wait is::

//     while (my_variable != what_i_want)
//         cpu_relax();

// asm volatile ("" ::: "memory")

/*
    https://stackoverflow.com/questions/67943540/why-can-asm-volatile-memory-serve-as-a-compiler-barrier

    If a variable is potentially read or written, it matters what order that happens in.
    The point of a "memory" clobber is to make sure the reads and/or writes in an asm statement
    happen at the right point in the program's execution.
*/

// void BarrierWait( std::atomic_int &barrier )
// {
//     barrier.fetch_sub(1, std::memory_order_seq_cst);

//     while (barrier.load(std::memory_order_seq_cst) > 0)
//     {
//         // lol does not work in loop
//         // asm volatile ("" ::: "memory");
//         asm volatile ("nop");
//     }
// }


namespace knl
{
    struct Barrier
    {
        std::atomic_int value{0};

        explicit
        Barrier( int count )
        {
            reset(count);
        }

        void reset( int count )
        {
            value.store(count);
        }

        void wait()
        {
            value--;
            while (value.load() > 0)
            {
                asm volatile ("nop");
            }
        }
    };
}






// class kthread_barrier_t
// {
// private:
//     const int threshold;          // Number of threads to wait for.
//     std::atomic<int> count;       // Remaining threads to wait for.
//     std::atomic<int> generation;  // Tracks barrier generations.

// public:
//     explicit kthread_barrier_t( int count )
//     : threshold(count), count(count), generation(0) {  };

//     void Wait()
//     {
//         int gen = generation.load();

//         if (--count == 0)
//         {
//             count.store(threshold); // Reset count for reuse.
//             generation.fetch_add(1); // Advance generation to release waiting threads.
//         }
        
//         else
//         {
//             while (generation.load() == gen)
//             {
//                 kthread::yield(); // Avoid busy waiting.
//             }
//         }
//     }

// };

