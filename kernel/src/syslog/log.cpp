#include <kernel/log.hpp>
#include <arch/io.hpp>
#include <stdio.h>

#ifdef SYSLOG_QEMU
#define LOG_BUFLEN 1024

static int indent = 0;
static char buf[LOG_BUFLEN];

void syslog::pushIndent( int n ) { indent += n; }
void syslog::popIndent ( int n ) { indent -= n; }


void
syslog::_putIndent()
{
    for (int i=0; i<indent; i++)
        IO::out8(IO::COM1, ' ');
}


int syslog::getIndent()
{
    return indent;
}


void
syslog::putIndent()
{
    if (!enabled)
        return;
    std::lock_guard lock(m_mutex);
    syslog::_putIndent();
}

void
syslog::_print( const char *fmt, ... )
{
    if (!enabled)
        return;

    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    for (int i=0; i<n; i++)
        IO::out8(IO::COM1, buf[i]);
}



void
syslog::vprint( const char *fmt, va_list vlist )
{
    if (!enabled) return;
    std::lock_guard lock(m_mutex);

    int n = vsnprintf(buf, sizeof(buf), fmt, vlist);
    for (int i=0; i<n; i++)
        IO::out8(IO::COM1, buf[i]);
}


void
syslog::print( const char *fmt, ... )
{
    // if (!enabled)
    //     return;
    // std::lock_guard lock(m_mutex);
    va_list vlist;
    va_start(vlist, fmt);
    syslog::vprint(fmt, vlist);
    va_end(vlist);
}


void
syslog::println( const char *fmt, ... )
{
    if (!enabled) return;
    std::lock_guard lock(m_mutex);

    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    for (int i=0; i<n; i++)
        IO::out8(IO::COM1, buf[i]);
    IO::out8(IO::COM1, '\n');
}

void
syslog::operator()( const char *fmt, ... )
{
    if (!enabled)
        return;
    std::lock_guard lock(m_mutex);

    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    for (int i=0; i<indent; i++)
        IO::out8(IO::COM1, ' ');

    for (int i=0; i<n; i++)
        IO::out8(IO::COM1, buf[i]);
    IO::out8(IO::COM1, '\n');

}

#endif
