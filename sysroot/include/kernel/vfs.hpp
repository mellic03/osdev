#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "vfsentry.hpp"


enum vfsFileStatus_
{
    vfsFileStatus_INVALID = 0,
    vfsFileStatus_GOOD,
    vfsFileStatus_DIRTY,
};

enum vfsFileType_
{
    vfsFileType_DUMMY = 0,
    vfsFileType_RAW,
    vfsFileType_FONT,
};

enum vfsFileFlag_
{
    vfsFileFlag_None       = 1<<0,
    vfsFileFlag_Virtual    = 1<<1,
    vfsFileFlag_Stream     = 1<<2,
    vfsFileFlag_Executable = 1<<3,
    vfsFileFlag_MaxBit     = 1<<3
};


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
        case vfsFileStatus_INVALID:     return "vfsFileStatus_INVALID";
        case vfsFileStatus_GOOD:        return "vfsFileStatus_GOOD";
        case vfsFileStatus_DIRTY:       return "vfsFileStatus_DIRTY";
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


inline const char *vfsFileTypeStr( uint8_t type )
{
    switch (type)
    {
        default:                        return "INVALID";
        case vfsFileType_DUMMY:         return "vfsFileType_DUMMY";
        case vfsFileType_RAW:           return "vfsFileType_RAW";
        case vfsFileType_FONT:          return "vfsFileType_FONT";
    }
};
