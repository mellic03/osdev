#include "spinlock.hpp"
#include <atomic>


class Semaphore
{
protected:
    lock_t value = 0;
    lock_t lock = 0;

    class SemaphoreBlocker : public ThreadBlocker
    {
        friend class Semaphore;
    public:
        SemaphoreBlocker* next = nullptr;
        SemaphoreBlocker* prev = nullptr;

        Semaphore* semaphore = nullptr;

        inline SemaphoreBlocker(Semaphore* sema) : semaphore(sema) {

        }

        void Interrupt(){
            interrupted = true;
            shouldBlock = false;

            acquireLock(&lock);
            if(semaphore){
                semaphore->blocked.remove(this);

                semaphore = nullptr;
            }

            if(thread){
                thread->Unblock();
            }
            thread = nullptr;
            releaseLock(&lock);
        }

        void Unblock(){
            shouldBlock = false;

            acquireLock(&lock);
            if(semaphore){
                semaphore->blocked.remove(this);
            }
            semaphore = nullptr;

            if(thread){
                thread->Unblock();
            }
            releaseLock(&lock);
        }

        ~SemaphoreBlocker(){
            if(semaphore){
                semaphore->blocked.remove(this);
            }
        }
    };
    

    FastList<SemaphoreBlocker*> blocked;

public:
    Semaphore(int val) : value(val) {};

    inline void SetValue(int val){
        value = val;
    }

    inline lock_t GetValue(){
        return value;
    }

    [[nodiscard]] bool Wait();
    [[nodiscard]] bool WaitTimeout(long& timeout);

    void Signal();
};

class ReadWriteLock
{
    unsigned activeReaders = 0;
    lock_t fileLock = 0;
    lock_t lock = 0;

    bool writerAcquiredLock = false; // Whether or not the writer has acquired lock (but not fileLock) 

    FastList<Thread*> readers;
    FastList<Thread*> writers;
public:

    ALWAYS_INLINE ReadWriteLock() {}

    ALWAYS_INLINE void AcquireRead(){
        acquireLock(&lock);

        if(__atomic_add_fetch(&activeReaders, 1, __ATOMIC_ACQUIRE) == 1){ // We are the first reader
            acquireLock(&fileLock);
        }

        releaseLock(&lock);
    }

    ALWAYS_INLINE void AcquireWrite(){
        acquireLock(&lock); // Stop more threads from reading
        acquireLock(&fileLock);
    }

    ALWAYS_INLINE bool TryAcquireWrite(){
        if(!writerAcquiredLock && acquireTestLock(&lock)){ // Stop more threads from reading
            return true;
        }
        writerAcquiredLock = true; // No need to acquire lock next time when we come back

        if(acquireTestLock(&fileLock)){
            return true;
        }
        writerAcquiredLock = false;

        return false;
    }

    ALWAYS_INLINE void ReleaseRead(){
        if(__atomic_sub_fetch(&activeReaders, 1, __ATOMIC_RELEASE) == 0){
            releaseLock(&fileLock);
        }
    }

    ALWAYS_INLINE void ReleaseWrite(){
        releaseLock(&fileLock);
        releaseLock(&lock);
    }

    ALWAYS_INLINE bool IsWriteLocked() const { return lock && activeReaders == 0; }
};






bool Semaphore::Wait()
{
    assert(CheckInterrupts());

    acquireLock(&lock);
    __sync_fetch_and_sub(&value, 1);

    if (value < 0) {
        SemaphoreBlocker blocker(this);
        blocked.add_back(&blocker);

        releaseLock(&lock);

        return Thread::Current()->Block(&blocker);
    }
    releaseLock(&lock);

    return false;
}

bool Semaphore::WaitTimeout(long& timeout) {
    acquireLock(&lock);
    __sync_fetch_and_sub(&value, 1);

    if (value < 0) {
        SemaphoreBlocker blocker(this);
        blocked.add_back(&blocker);

        releaseLock(&lock);

        return Thread::Current()->Block(&blocker, timeout);
    }
    releaseLock(&lock);

    return false;
}

void Semaphore::Signal() {
    acquireLock(&lock);

    __sync_fetch_and_add(&value, 1);
    if (blocked.get_length() > 0) {
        blocked.get_front()->Unblock();
    }

    releaseLock(&lock);
}

