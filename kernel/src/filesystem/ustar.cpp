#include <filesystem/ustar.hpp>
#include <kernel/kstring.h>
#include <string.h>

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

#include <kernel/log.hpp>

// void ustar::forEach( void *archive, void(*action)(const char*, void*, size_t) )
void ustar::forEach( void *addr, std::function<void(const char*, void*, size_t)> action )
{
    auto *ptr = (unsigned char*)(addr);
    while (!memcmp(ptr + 257, "ustar", 5))
    {
        size_t fsize = oct2bin(ptr + 0x7c, 11);
        action((const char*)ptr, (void*)(ptr+512), fsize);
        ptr += (((fsize + 511) / 512) + 1) * 512;
    }
}


void ustar::list( void *addr )
{
    syslog log("ustar::list");

    auto *dirname = (const char*)addr;
    auto *slash = seek_ch(dirname, '/');
    size_t len = slash-dirname;

    if (*slash == '\0')
    {
        log("Not a directory!");
        return;
    }

    char root[128];
    strncpy(root, dirname, slash-dirname);
    log("%s/", root);

    ustar::forEach(addr, [&log, root, len](const char *name, void*, size_t) {
        if (strncmp(root, name, len) != 0)
            return;
        log(name);
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


void *ustar::find( void *archive, const char *filename )
{
    auto *ptr = (unsigned char*)(archive);
    while (!memcmp(ptr + 257, "ustar", 5))
    {
        size_t fsize = oct2bin(ptr + 0x7c, 11);
        if (!memcmp(ptr, filename, strlen(filename)+1))
            return (void*)(ptr + 512);
        ptr += (((fsize + 511) / 512) + 1) * 512;
    }
    return nullptr;
}