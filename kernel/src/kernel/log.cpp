#include <kernel/log.hpp>
#include <arch/io.hpp>
#include <stdio.h>

#define LOG_BUFLEN 1024

static int indent = 0;
static char buf[LOG_BUFLEN];

void syslog::pushIndent( int n ) { indent += n; }
void syslog::popIndent ( int n ) { indent -= n; }


void
syslog::_putIndent()
{
    for (int i=0; i<indent; i++)
        IO::outb(IO::COM1, ' ');
}

void
syslog::putIndent()
{
    if (!enabled)
        return;
    m_mutex.lock();
    syslog::_putIndent();
    m_mutex.unlock();
}


void
syslog::print( const char *fmt, ... )
{
    if (!enabled)
        return;

    va_list args;
    va_start(args, fmt);
    int n = vsprintf(buf, fmt, args);
    va_end(args);

    for (int i=0; i<n&&i<LOG_BUFLEN; i++)
        IO::outb(IO::COM1, buf[i]);
}


void
syslog::println( const char *fmt, ... )
{
    if (!enabled)
        return;
    m_mutex.lock();

    va_list args;
    va_start(args, fmt);
    int n = vsprintf(buf, fmt, args);
    va_end(args);

    for (int i=0; i<n&&i<LOG_BUFLEN; i++)
        IO::outb(IO::COM1, buf[i]);
    IO::outb(IO::COM1, '\n');

    m_mutex.unlock();
}


void
syslog::operator()( const char *fmt, ... )
{
    if (!enabled)
        return;
    // std::lock_guard lock(m_mutex);
    m_mutex.lock();

    va_list args;
    va_start(args, fmt);
    int n = vsprintf(buf, fmt, args);
    va_end(args);

    for (int i=0; i<indent; i++)
        IO::outb(IO::COM1, ' ');

    for (int i=0; i<n; i++)
        IO::outb(IO::COM1, buf[i]);
    IO::outb(IO::COM1, '\n');

    m_mutex.unlock();
}
