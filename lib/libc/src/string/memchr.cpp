#include <string.h>


void *memchr( const void *ptr, int ch, size_t count )
{
    uint8_t *p = (uint8_t*)ptr;
    uint8_t  c = uint8_t(ch);

    for (size_t i=0; i<count; i++)
    {
        if (p[i] == c)
        {
            return (void*)(p);
        }
    }

    return nullptr;
}
