#include <libc++>
#include <cstdlib>


void *operator new( size_t nbytes )
{
    return malloc(nbytes);
}

void *operator new[]( size_t nbytes )
{
    return malloc(nbytes);
}

void operator delete( void *p )
{
    free(p);
}

void operator delete( void *p, size_t )
{
    free(p);
}

void operator delete[]( void *p )
{
    free(p);
}

void operator delete[]( void *p, size_t )
{
    free(p);
}

