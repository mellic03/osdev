#include <stdio.h>
#include <stdlib.h>

#ifndef __is_kernel
extern "C"
{

    int ulibc_add( int x, int y )
    {
        return x + y;
    }

    void _dl_init()
    {
        printf("test");
    }

    namespace __cxxabiv1
    {
        int __cxa_guard_acquire( uint64_t *g )
        {
            return !__atomic_load_n(g, __ATOMIC_SEQ_CST)
                && !__atomic_exchange_n(g, 0, __ATOMIC_SEQ_CST);
        }

        void __cxa_guard_release( uint64_t *g )
        {
            __atomic_store_n(g, 1, __ATOMIC_SEQ_CST);
        }

        void __cxa_guard_abort( uint64_t* )
        {
            
        }
    }

}
#endif
