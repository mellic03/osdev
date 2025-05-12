#pragma once
#include <atomic>
#include <kthread.hpp>


class SMPBarrier
{
private:
    const int threshold;          // Number of threads to wait for.
    std::atomic<int> count;       // Remaining threads to wait for.
    std::atomic<int> generation;  // Tracks barrier generations.

public:
    explicit SMPBarrier( int count )
    : threshold(count), count(count), generation(0) {  };

    void Wait()
    {
        int gen = generation.load();

        if (--count == 0)
        {
            count.store(threshold); // Reset count for reuse.
            generation.fetch_add(1); // Advance generation to release waiting threads.
        }
        
        else
        {
            while (generation.load() == gen)
            {
                kthread::yield(); // Avoid busy waiting.
            }
        }
    }

};

