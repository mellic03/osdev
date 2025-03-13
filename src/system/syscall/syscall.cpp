#include "./syscall.hpp"
#include "system/drivers/serial.hpp"

#include <stdio.h>


using namespace idk;

SysRequest  *internal::__sysreq = nullptr;
SysResponse *internal::__sysres = nullptr;


void idk::internal::__syscall_init( idk::base_allocator *mem )
{
    __sysreq = mem->alloc<SysRequest>();
    __sysres = mem->alloc<SysResponse>();
}


const SysResponse &idk::syscall( const SysRequest &req )
{
    serial_printf("\n");
    serial_printf("[idk::syscall] A\n");

    *internal::__sysreq = req;
    serial_printf("[idk::syscall] B\n");

    asm volatile ("int $80");
    serial_printf("[idk::syscall] C\n");

    return *internal::__sysres;
}


