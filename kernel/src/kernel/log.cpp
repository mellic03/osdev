#include <kernel/log.hpp>
#include <kernel/ioport.hpp>
#include <stdio.h>

static int  indent  = 0;
static char buf[256];

void syslog::pushIndent( int n ) { indent += n; }
void syslog::popIndent ( int n ) { indent -= n; }


void
syslog::printf( const char *fmt, ... )
{
    va_list args;
    va_start(args, fmt);
    int n = vsprintf(buf, fmt, args);
    va_end(args);

    for (int i=0; i<n; i++)
        IO::outb(IO::COM1, buf[i]);
}


void
syslog::println( const char *fmt, ... )
{
    m_mutex.lock();
    va_list args;
    va_start(args, fmt);
    int n = vsprintf(buf, fmt, args);
    va_end(args);

    for (int i=0; i<n; i++)
        IO::outb(IO::COM1, buf[i]);
    IO::outb(IO::COM1, '\n');
    m_mutex.unlock();
}


void
syslog::print( const char *fmt, ... )
{
    va_list args;
    va_start(args, fmt);
    int n = vsprintf(buf, fmt, args);
    va_end(args);

    for (int i=0; i<indent; i++)
    {
        IO::outb(IO::COM1, ' ');
    }

    for (int i=0; i<n; i++)
    {
        IO::outb(IO::COM1, buf[i]);
    }

    // IO::outb(IO::COM1, '\0');
}


void
syslog::operator()( const char *fmt, ... )
{
    std::lock_guard lock(m_mutex);

    va_list args;
    va_start(args, fmt);
    int n = vsprintf(buf, fmt, args);
    va_end(args);
    
    for (int i=0; i<indent; i++)
    {
        IO::outb(IO::COM1, ' ');
    }

    for (int i=0; i<n; i++)
    {
        IO::outb(IO::COM1, buf[i]);
    }

    IO::outb(IO::COM1, '\n');
    // IO::outb(IO::COM1, '\0');
}
