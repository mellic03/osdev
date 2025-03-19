#include <kmalloc.h>

void *operator new( size_t nbytes )
{
    return kmalloc(nbytes);
}

void *operator new[]( size_t nbytes )
{
    return kmalloc(nbytes);
}

void operator delete( void *p )
{
    kfree(p);
}

void operator delete( void *p, size_t )
{
    kfree(p);
}

void operator delete[]( void *p )
{
    kfree(p);
}

void operator delete[]( void *p, size_t )
{
    kfree(p);
}


