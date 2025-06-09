#include <stdlib.h>

#ifdef __is_kernel
    #include <kthread.hpp>
#else
    #include <sys/syscall.hpp>
#endif


static void (*atexit_func)(void) = nullptr;

void atexit( void (*func)(void) )
{
    atexit_func = func;
}


void exit( int )
{
    if (atexit_func)
    {
        atexit_func();
    }

    #ifdef __is_kernel
        // kthread::exit();
    #else
        knl::syscall(SysNo_Exit, nullptr, nullptr);
    #endif
}


void abort()
{

}

