#include <sys/syscall.hpp>
#include <sys/interrupt.hpp>
#include <cpu/cpu.hpp>
#include <kassert.h>
#include <kmalloc.h>


void knl::syscall( SysNo_ sysno, const void *req, void *res )
{
    cpu_t *cpu = SMP::this_cpu();

    cpu->syscall_no  = sysno;
    cpu->syscall_req = (uintptr_t)req;
    cpu->syscall_res = (uintptr_t)res;

    knl::interrupt<IntNo_Syscall>();
}
