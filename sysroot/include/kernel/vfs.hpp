#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "vfsentry.hpp"


namespace kfilesystem
{
    vfsFileEntry *vfsInsertFile( const char*, void *addr=0, size_t size=0,
                                 uint32_t flags=vfsFileFlag_None );

    vfsFileEntry *vfsFindFile( const char *path );
    vfsFileEntry *vfsFindFile( vfsDirEntry *cwd, const char *path );

    vfsDirEntry  *vfsInsertDirectory( const char* );
    vfsDirEntry  *vfsFindDirectory( const char* );
    vfsDirEntry  *vfsFindDirectory( vfsDirEntry *cwd, const char* );


    template <typename T>
    vfsFileEntry *vfsInsertFile( const char *path, size_t count,
                                 uint32_t flags=vfsFileFlag_None )
    {
        return vfsInsertFile(path, new T[count], count*sizeof(T), flags);
    }


    template <typename T>
    T vfsFindFile( const char *path )
    {
        return static_cast<T>( vfsFindFile(path)->addr );
    }

    template <typename T>
    T vfsFindFile( vfsDirEntry *cwd, const char *path )
    {
        return static_cast<T>( vfsFindFile(cwd, path)->addr );
    }

}






inline const char *vfsFileStatusStr( uint32_t bit )
{
    switch (bit)
    {
        default:                        return "INVALID";
        case vfsFileStatus_Invalid:     return "vfsFileStatus_Invalid";
        case vfsFileStatus_Good:        return "vfsFileStatus_Good";
        case vfsFileStatus_Dirty:       return "vfsFileStatus_Dirty";
    }
};


inline const char *vfsFileFlagStr( uint32_t bit )
{
    switch (bit)
    {
        default:                     return "INVALID";
        case vfsFileFlag_None:       return "vfsFileFlag_None";
        case vfsFileFlag_Virtual:    return "vfsFileFlag_Virtual";
        case vfsFileFlag_Stream:     return "vfsFileFlag_Stream";
        case vfsFileFlag_Executable: return "vfsFileFlag_Executable";
    }
};

