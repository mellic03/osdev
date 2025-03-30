#include <ksysc.h>
#include <kthread.hpp>

extern "C"
{
    extern void __libk_syscall( void* );
}

void libk_syscall( struct ksysc_request *req )
{
    __libk_syscall(req);
    kthread::yield();
}
