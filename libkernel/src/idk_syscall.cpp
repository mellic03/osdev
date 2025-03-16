#include <idk_syscall.h>
#include <idk_interrupt.hpp>



void idk_Syscall( uint32_t *type, uint32_t *flags, char buf[256] )
{
    idk::Interrupt(INT_SYSCALL);
}


