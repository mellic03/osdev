#include <stdc++/cstdio.hpp>


struct std::FILE
{
    uint32_t *baseptr;
    uint32_t  size;
};


std::FILE*
std::fopen( const char *filename, const char *mode )
{
    std::FILE *fh = nullptr;

    if (filename == 0 || mode[0] == 'a')
    {
        return fh;
    }

    return fh;
}


