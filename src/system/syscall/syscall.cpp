#include "syscall.hpp"
#include "system/drivers/serial.hpp"
#include "system/interrupt/interrupt.hpp"
#include <stdio.h>


using namespace idk;

SysRequest  *internal::__sysreq = nullptr;
SysResponse *internal::__sysres = nullptr;


void idk::internal::__syscall_init( idk::linear_allocator &mem )
{
    __sysreq = mem.alloc<SysRequest>();
    __sysres = mem.alloc<SysResponse>();
}


// const SysResponse &idk::Syscall( const SysRequest &req )
// {
//     serial_printf("[idk::syscall] A\n");

//     *internal::__sysreq = req;
//     serial_printf("[idk::syscall] B\n");

//     idk::Interrupt(Exception::SYSCALL);
//     serial_printf("[idk::syscall] C\n");

//     return *internal::__sysres;
// }


void idk::Syscall( uint32_t *type, uint32_t *flags, char buf[256] )
{
    auto &req = *internal::__sysreq;
    auto &res = *internal::__sysres;

    req = SysRequest(*type, *flags, buf);
    idk::Interrupt(Exception::SYSCALL);

    *type  = res.type;
    *flags = res.flags;
}
