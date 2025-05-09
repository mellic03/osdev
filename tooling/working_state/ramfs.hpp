#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../include/kernel/bitmap_allocator.hpp"


class RamFS;
static constexpr size_t RFS_BLOCK_SIZE = 1024;


enum rfsEntry_: uint32_t
{
    rfsEntry_Invalid = 0,
    rfsEntry_Directory,
    rfsEntry_File,
};


enum rfsFlag_: uint32_t
{
    rfsFlag_None    = 0,
    rfsFlag_Symlink = 1<<0,
};


struct rfsBlock
{
    int32_t  next_id; // 4
    uint32_t padding; // 4
    uint8_t *tail;    // 8
    uint8_t *end;     // 8
    // 4 + 4 + 8 + 8 == 3*8 == 24 bytes
    uint8_t  data[RFS_BLOCK_SIZE - 3*sizeof(uint64_t)];

    rfsBlock()
    {
        next_id = -1;
        padding = 0;
        tail    = data;
        end     = (uint8_t*)this + RFS_BLOCK_SIZE;
    }

};


struct rfsEntry
{
    static constexpr uint32_t etype = rfsEntry_Invalid;

    uint32_t type;
    uint32_t flags;
    char     name[32];
    int32_t  entry_id;
    int32_t  parent_id;
    int32_t  block_id;

    uint32_t nEntries;
    uint32_t padding;
};



struct rfsHeader
{
    size_t totalSize;
    size_t blockSize;

    size_t entryBitmapOffset;
    size_t entryHeapOffset;
    size_t entryCapacity;

    size_t blockBitmapOffset;
    size_t blockHeapOffset;
    size_t blockCapacity;

    size_t rootOffset;
};



class RamFS
{
private:
    static constexpr size_t rfsEntryCapacity = 512;
    static constexpr size_t rfsBlockCapacity = 32*1024;

    using entryBitmap_type = idk::BitMap<rfsEntryCapacity>;
    using blockBitmap_type = idk::BitMap<rfsBlockCapacity>;

    template <typename bitmap_type>
    int32_t allocID( bitmap_type &bitmap )
    {
        for (size_t i=0; i<bitmap.nbits(); i++)
        {
            if (bitmap.is_unset(i))
            {
                bitmap.set(i);
                return i;
            }
        }
        return -1;
    }
    int32_t allocEntryID() { return allocID(*m_entryBitmap); }
    int32_t allocBlock();

    rfsEntry *find_child( rfsEntry*, const char *name );


public:
    rfsHeader                *m_header;
    uintptr_t                   m_base;
    uintptr_t                   m_end;

    entryBitmap_type           *m_entryBitmap;
    blockBitmap_type           *m_blockBitmap;

    rfsEntry                 *m_entryHeap;
    uint8_t                    *m_blockHeap;

    rfsEntry                 *m_root;

    // RamFS() {  };
    RamFS( void *base );
    RamFS( void *base, size_t size );

    void        formatDisk();

    rfsEntry *allocEntry( uint32_t type, const char *name );

    rfsEntry *getEntry( int32_t id );
    rfsBlock *getBlock( int32_t id );
    void        addToDirectory( rfsEntry *dir, rfsEntry *entry );
    // void        printAbsolutePath( rfsEntry *entry );

    rfsEntry *open( const char *path );
    void        walk( int32_t entry_id, int depth=0 );

    size_t block_write( rfsBlock *block, const uint8_t *srcbuf, size_t nbytes );
    size_t file_write( rfsEntry *file, const uint8_t *srcbuf, size_t nbytes );
};



