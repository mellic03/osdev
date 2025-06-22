#include <stdio.h>
#include <stddef.h>
#include <stdint.h>


#ifdef __is_kernel
    int printf( const char *fmt, ... )
    {
        va_list args;
        va_start (args, fmt);
        int n = vprintf(fmt, args);
        va_end(args);
        return n;
    }


    int fprintf( FILE *fh, const char *fmt, ... )
    {
        va_list args;
        va_start (args, fmt);
        int n = vfprintf(fh, fmt, args);
        va_end(args);
        return n;
    }


    int sprintf( char *buf, const char *fmt, ... )
    {
        va_list args;
        va_start (args, fmt);
        int n = vsprintf(buf, fmt, args);
        va_end(args);
        return n;
    }


    int snprintf( char *buf, size_t bufsize, const char *fmt, ... )
    {
        va_list args;
        va_start (args, fmt);
        int n = vsnprintf(buf, bufsize, fmt, args);
        va_end(args);
        return n;
    }


    int asprintf( char **buf, const char *fmt, ... )
    {
        va_list args;
        va_start (args, fmt);
        int n = vsprintf(*buf, fmt, args);
        va_end(args);
        return n;
    }


#else
    // #include <sys/syscall.hpp>
    #include <sys/interrupt.hpp>

    int printf( const char *fmt, ... )
    {
        static int res = fmt[0];
        knl::interrupt<IntNo_Syscall>();
        // knl::syscall(SysNo_PrintTest, fmt, &res);
        return res;
    }

    int fprintf( FILE*, const char*, ... ) { return -1; }
    int sprintf( char*, const char*, ... ) { return -1; }
    int snprintf( char*, size_t, const char*, ... ) { return -1; }
    int asprintf( char *, const char*, ... ) { return -1; }


#endif

