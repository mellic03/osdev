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


enum ramfsEntry_: uint32_t
{
    ramfsEntry_Invalid = 0,
    ramfsEntry_Directory,
    ramfsEntry_File,
};


enum ramfsFlag_: uint32_t
{
    ramfsFlag_None    = 0,
    ramfsFlag_Symlink = 1<<0,
};


struct ramfsBlock
{
    int32_t  next_id; // 4
    uint32_t padding; // 4
    uint8_t *tail;    // 8
    uint8_t *end;     // 8
    // 4 + 4 + 8 + 8 == 3*8 == 24 bytes
    uint8_t  data[RFS_BLOCK_SIZE - 3*sizeof(uint64_t)];

    ramfsBlock()
    {
        next_id = -1;
        padding = 0;
        tail    = data;
        end     = (uint8_t*)this + RFS_BLOCK_SIZE;
    }

};


struct ramfsEntry
{
    static constexpr uint32_t etype = ramfsEntry_Invalid;

    uint32_t type;
    uint32_t flags;
    char     name[32];
    int32_t  entry_id;
    int32_t  parent_id;
    int32_t  block_id;

    uint32_t nEntries;
    uint32_t padding;
};



struct ramfsHeader
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

    ramfsEntry *find_child( ramfsEntry*, const char *name );


public:
    ramfsHeader                *m_header;
    uintptr_t                   m_base;
    uintptr_t                   m_end;

    entryBitmap_type           *m_entryBitmap;
    blockBitmap_type           *m_blockBitmap;

    ramfsEntry                 *m_entryHeap;
    uint8_t                    *m_blockHeap;

    ramfsEntry                 *m_root;

    // RamFS() {  };
    RamFS( void *base );
    RamFS( void *base, size_t size );

    void        formatDisk();

    ramfsEntry *allocEntry( uint32_t type, const char *name );

    ramfsEntry *getEntry( int32_t id );
    ramfsBlock *getBlock( int32_t id );
    void        addToDirectory( ramfsEntry *dir, ramfsEntry *entry );
    // void        printAbsolutePath( ramfsEntry *entry );

    ramfsEntry *open( const char *path );
    void        walk( int32_t entry_id, int depth=0 );

    size_t block_write( ramfsBlock *block, const uint8_t *srcbuf, size_t nbytes );
    size_t file_write( ramfsEntry *file, const uint8_t *srcbuf, size_t nbytes );
};



