#include <filesystem/ustar.hpp>
#include <kernel/kstring.h>
#include <kernel/log.hpp>
#include <string.h>
#include <stdio.h>

size_t oct2bin( unsigned char *str, int size )
{
    size_t n = 0;
    unsigned char *c = str;
    while (size-- > 0)
    {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}


static bool ustar_isChild( void *A, void *B )
{
    if (A == B)
        return false;
    auto *nameA = (const char*)A;
    auto *nameB = (const char*)B;
    auto *slash = seek_ch(nameA, '/');
    size_t len = slash-nameA;
    if (*slash == '\0')
        return false;
    return strncmp(nameA, nameB, len) == 0;
}


// void ustar::forEach( void *archive, void(*action)(const char*, void*, size_t) )
void ustar::forEach( void *addr, std::function<void(void*, size_t)> action )
{
    auto *ptr = (unsigned char*)(addr);
    while (!memcmp(ptr + 257, "ustar", 5))
    {
        size_t fsize = oct2bin(ptr + 0x7c, 11);

        // auto *name = (const char*)ptr;
        // if (name[strlen(name)-1] != '/')
        if (ustar_isChild(addr, ptr))
            action((void*)ptr, fsize);

        ptr += (((fsize + 511) / 512) + 1) * 512;
    }
}



void ustar::listChilren( void *tar )
{
    syslog log("ustar::listChilren");
    ustar::forEach(tar, [&log, tar](void *child, size_t) {
        if (ustar_isChild(tar, (void*)child) == false)
            return;
        log((const char*)child);
    });
}

void ustar::listAll( void *tar )
{
    syslog log("ustar::listAll");
    ustar::forEach(tar, [&log](void *child, size_t) {
        log("%s", (const char*)child);
    });
}


// bool ustar::find( void *archive, const char *filename, void *&addr, size_t &fsize )
// {
//     auto *ptr = (unsigned char*)(archive);
//     while (!memcmp(ptr + 257, "ustar", 5))
//     {
//         fsize = oct2bin(ptr + 0x7c, 11);
//         if (!memcmp(ptr, filename, strlen(filename)+1))
//         {
//             addr = (void*)(ptr + 512);
//             return true;
//         }
//         ptr += (((fsize + 511) / 512) + 1) * 512;
//     }
//     return false;
// }

void *ustar::find( void *archive, const char *name )
{
    auto *ptr = (unsigned char*)(archive);
    while (strncmp((const char*)(ptr+257), "ustar", 5) == 0)
    {
        size_t size = oct2bin(ptr + 0x7c, 11);
        if (strncmp((const char*)ptr, name, strlen(name)) == 0)
            return (void*)ptr;
            // return (void*)(ptr + 512);
        ptr += (((size + 511) / 512) + 1) * 512;
    }
    return nullptr;
}

