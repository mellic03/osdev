#pragma once

#include <stdio.h>
#include <kernel/log.hpp>
#include "../tty.hpp"


void kshell_main( void *arg );


namespace KShell
{
    static constexpr int MAX_ARG_COUNT  = 16;
    static constexpr int MAX_ARG_LENGTH = 32;

    extern int kshell_indent;
    void pushIndent();
    void popIndent();

    template <typename... Args>
    char *kssprintln( char *dst, const char *fmt, Args... args )
    {
        for (int i=0; i<KShell::kshell_indent; i++)
            dst += sprintf(dst, " ");
        dst += sprintf(dst, fmt, args...);
        dst += sprintf(dst, "\n");
        return dst;
    }

    template <typename... Args>
    char *kssprintln( char *dst, syslog &log, const char *fmt, Args... args )
    {
        for (int i=0; i<KShell::kshell_indent; i++)
            dst += sprintf(dst, " ");

        dst += sprintf(dst, fmt, args...);
        dst += sprintf(dst, "\n");
        log.print(fmt, args...);
        log.print("\n");

        return dst;
    }

    template <typename... Args>
    char *kssprint( char *dst, syslog &log, const char *fmt, Args... args )
    {
        for (int i=0; i<KShell::kshell_indent; i++)
            dst += sprintf(dst, " ");

        dst += sprintf(dst, fmt, args...);
        log.print(fmt, args...);

        return dst;
    }
}


