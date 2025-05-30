#include <stdlib.h>

#ifdef __is_kernel
    #include <sys/sys.hpp>
#else
    #include <sys/syscall.hpp>
#endif


int system( const char *cmd )
{
    #ifdef __is_kernel
        return knl::shell_exec(cmd);
    #else
        static int res = 0;
        knl::syscall(SysNo_ShellExec, cmd, &res);
        return res;
    #endif
}

