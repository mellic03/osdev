#include "syscall.hpp"
#include <sys/interrupt.hpp>
#include <cpu/cpu.hpp>
#include <kassert.h>
#include <kmalloc.h>


// void syscall( SysNo_ sysno, const sysreq_t *req, sysres_t *res )
// {
//     cpu_t *cpu = SMP::this_cpu();

//     cpu->syscall_no  = sysno;
//     cpu->syscall_req = (uintptr_t)req;
//     cpu->syscall_res = (uintptr_t)res;

//     knl::interrupt<IntNo_Syscall>();
// }


// static void handle_FileOpen( sysreq_t *arg, sysres_t *res )
// {
//     auto *req  = (sysreq_FileOpen*)arg;
//     char *path = (char*)(req->path);
//     auto *fh   = vfs2::open(path, req->nbytes, nullptr);
//     res->addr  = (void*)fh;
// }


// static void handle_MemAlloc( sysreq_t *arg, sysres_t *res )
// {
//     auto *req = (sysreq_MemAlloc*)arg;
//     res->addr = kmalloc(req->nbytes);
// }



// void knl::syscallISR( intframe_t* )
// {
//     cpu_t *cpu = SMP::this_cpu();

//     auto sysno = cpu->syscall_no;
//     auto *req = (sysreq_t*)(cpu->syscall_req);
//     auto *res = (sysres_t*)(cpu->syscall_res);

//     switch (sysno)
//     {
//         default: kpanic("Invalid sysno"); break;
//         case SysNo_FileOpen: handle_FileOpen(req, res); break;
//         case SysNo_MemAlloc: handle_MemAlloc(req, res); break;
//     }
// }



// using hfn_type = void (*)(sysreq_t*);
// static hfn_type lookup[SysNo_SysNoCount];

// __attribute__((constructor))
// static void lookup_init()
// {
//     for (int i=0; i<SysNo_SysNoCount; i++)
//         lookup[i] = nullptr;
    
//     lookup[SysNo_FileOpen] = handle_FileOpen;
//     lookup[SysNo_MemAlloc] = handle_MemAlloc;
    
// }





// #ifdef __cplusplus
//     extern "C" void *syscall( uint64_t sysno, void* );
// #else
// #endif




// enum SYSF_
// {
//     SYSF_FILE_ANY = 1,
//     SYSF_FILE_KDEVKEY,
//     SYSF_FILE_KDEVSCN,

// };


// struct ksysc_request
// {
//     uint32_t type;
//     uint32_t flags;
//     uint64_t size;
//     char     msg[64];
//     void    *data;
//     void    *res;
// };


// struct ksysc_window_response
// {
//     void *sde_ctx;
//     int *x, *y;
//     float *border;
// };

// KERNEL_EXTERN_ASM void libk_syscall( struct ksysc_request *req );


// #include "ZZZ.h"





