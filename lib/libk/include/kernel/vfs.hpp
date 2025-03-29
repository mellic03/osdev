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
    vfsFileType_EXECUTABLE,
    vfsFileType_STREAM,
};

enum vfsFileFlag_
{
    vfsFileFlag_PHYSICAL  = 1<<0,
    vfsFileFlag_VIRTUAL   = 1<<1,
};


namespace kfilesystem
{
    vfsFileEntry *vfsInsertFile( const char*, const char*, void *addr=0,
                                size_t size=0, uint8_t type=vfsFileType_RAW );

    vfsFileEntry *vfsFindFile( const char *fname );
    vfsFileEntry *vfsFindFile( vfsDirEntry *cwd, const char *fname );

    vfsDirEntry  *vfsInsertDirectory( const char* );
    vfsDirEntry  *vfsFindDirectory( const char* );
    vfsDirEntry  *vfsFindDirectory( vfsDirEntry *cwd, const char* );


    template <typename T>
    vfsFileEntry *vfsInsertFile( const char *dname, const char *fname, size_t count,
                                 uint8_t type=vfsFileType_RAW )
    {
        return vfsInsertFile(dname, fname, new T[count], count*sizeof(T), type);
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
        default:                        return "INVALID";
        case vfsFileFlag_PHYSICAL:      return "vfsFileFlag_PHYSICAL";
        case vfsFileFlag_VIRTUAL:       return "vfsFileFlag_VIRTUAL";
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
        case vfsFileType_EXECUTABLE:    return "vfsFileType_EXECUTABLE";
        case vfsFileType_STREAM:        return "vfsFileType_STREAM";
    }
};
