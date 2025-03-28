#pragma once

#include <stdio.h>
#include "../log/log.hpp"
#include "../tty.hpp"


void kshell_main( void *arg );


namespace KShell
{
    static constexpr int MAX_ARG_COUNT  = 16;
    static constexpr int MAX_ARG_LENGTH = 32;

    inline static int kshell_indent = 4;

    extern kTTY *kshell_tty;
    extern char  kshell_buf[128];

    template <typename... Args>
    char *kssprintf( char *dst, const char *fmt, Args... args )
    {
        for (int i=0; i<kshell_indent; i++)
            dst += sprintf(dst, " ");
        dst += sprintf(dst, fmt, args...);
        dst += sprintf(dst, "\n");
        return dst;
    }
}


