#include <libc++>
#include <cstdlib>


#ifdef __is_kernel
void libcpp::init()
{
    return;
}
#endif


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

