#include <kernel/log.hpp>
#include <kernel/ioport.hpp>
#include <stdio.h>
#include <mutex>

bool syslog::enabled = true;
static int  indent  = 0;
static char buf[256];
static std::mutex m_mutex;

void syslog::enable()  { enabled = true;  }
void syslog::disable() { enabled = false; }
void syslog::pushIndent( int n ) { indent += n; }
void syslog::popIndent ( int n ) { indent -= n; }


void
syslog::kprintf( const char *fmt, ... )
{
    m_mutex.lock();
    va_list args;
    va_start(args, fmt);
    int n = vsprintf(buf, fmt, args);
    va_end(args);

    for (int i=0; i<n; i++)
        IO::outb(IO::COM1, buf[i]);
    
    m_mutex.unlock();
}

void
syslog::print( const char *fmt, ... )
{
    if (!enabled)
        return;
    m_mutex.lock();

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
    m_mutex.unlock();

    // IO::outb(IO::COM1, '\0');
}


void
syslog::operator()( const char *fmt, ... )
{
    if (!enabled)
        return;

    m_mutex.lock();

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
    m_mutex.unlock();
}
