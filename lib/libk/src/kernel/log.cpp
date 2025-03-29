#include <kernel/log.hpp>
#include <kernel/ioport.hpp>
#include <stdio.h>

static bool enabled = true;
static int  indent  = 0;
static char buf[128];

void syslog::enable()  { enabled = true;  }
void syslog::disable() { enabled = false; }


syslog::syslog( const char *title )
{
    (*this)("[%s]", title);
    (*this)("{");
    indent += 4;
}


syslog::~syslog()
{
    indent -= 4;
    (*this)("}");
}


void
syslog::operator()( const char *fmt, ... )
{
    if (!enabled)
    {
        return;
    }

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
    IO::outb(IO::COM1, '\0');

}