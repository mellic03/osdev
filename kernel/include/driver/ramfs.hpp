#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <bitmap2.hpp>
// #include <kernel/bitmap_allocator.hpp>
#include <string.h>


class RamFS;

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
    int32_t  idx;
    int32_t  next_id;
    size_t   base;
    size_t   tail;
    size_t   end;
    uint8_t  data[1];

    ramfsBlock( size_t block_size )
    {
        idx     = 0;
        next_id = -1;
        base    = 0;
        tail    = 0;
        end     = block_size - offsetof(ramfsBlock, data);
    }
};


struct ramfsEntry
{
    uint32_t type;
    uint32_t flags;
    uint32_t size;
    size_t   cursor;
    char     name[32];
    int32_t  entry_id;
    int32_t  parent_id;
    int32_t  block_id;
    uint32_t entry_count;
};


struct ramfsHeader
{
    size_t size;
    size_t blockSize;

    size_t entryCapacity;
    size_t blockCapacity;

    size_t entryBitmapOffset;
    size_t blockBitmapOffset;
    
    size_t entryHeapOffset;
    size_t blockHeapOffset;

    size_t rootOffset;
};



class RamFS
{
private:
    template <typename bitmap_type>
    int32_t allocID( bitmap_type &bitmap )
    {
        for (size_t i=0; i<bitmap.nbits; i++)
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
    int32_t allocBlock( ramfsBlock *tail=nullptr );
    ramfsEntry *find_child( ramfsEntry*, const char *name );
    size_t block_write( ramfsBlock *block, const uint8_t *src, size_t nbytes );


public:
    ramfsHeader *m_header;
    uintptr_t    m_base;
    uintptr_t    m_end;

    idk_BitMap  *m_entryBitmap;
    idk_BitMap  *m_blockBitmap;

    ramfsEntry  *m_entryHeap;
    uint8_t     *m_blockHeap;

    ramfsEntry  *m_root;

    RamFS() {  };
    RamFS( void *base );
    RamFS( void *base, size_t size );

    void        formatDisk();
    void        loadDisk();
    void        addToDirectory( ramfsEntry *dir, ramfsEntry *entry );
    void        appendBlock( ramfsEntry *entry );

    ramfsEntry *allocEntry( uint32_t type, const char *name );
    ramfsEntry *getEntry( int32_t entry_id );
    ramfsBlock *getBlock( int32_t block_id );
    ramfsBlock *getLastBlock( int32_t block_id );
    ramfsBlock *getIndexedBlock( int32_t block_id );

    ramfsEntry *open( const char *path );
    void        walk( int32_t entry_id, int depth=0 );

    size_t      fwrite( ramfsEntry *file, const uint8_t *src, size_t nbytes );
    // size_t      fread( ramfsEntry *file, void *dst, size_t nbytes );
    // size_t      ftell( ramfsEntry *file );

    // rfsEntryIterator begin() { return rfsEntryIterator(*this, m_root, 0);  };
    // rfsEntryIterator end()   { return rfsEntryIterator(*this, nullptr, 0); };
};



namespace rfs
{
    void init( void *base );

    ramfsEntry *open( const char *path );
    size_t      fwrite( ramfsEntry *file, const uint8_t *src, size_t nbytes );
    // size_t      fread ( ramfsEntry *file, uint8_t *dst, size_t nbytes );

    size_t      ftell( ramfsEntry *fh ); //           { return fh->cursor; }
    size_t      fseek( ramfsEntry *fh, size_t p ); // { fh->cursor = p; return p;  }
}




// struct rfsEntryIterator
// {
//     RamFS &rfs;
//     ramfsEntry *entry;
//     uint32_t entryno;

//     rfsEntryIterator( RamFS &fs, ramfsEntry *E, uint32_t eno )
//     :   rfs(fs), entry(E), entryno(eno)
//     {
//         if (entry && entryno > entry->entry_count)
//         {
//             entry = nullptr;
//             entryno = 0;
//         }
//     };

//     rfsEntryIterator( const rfsEntryIterator &other )
//     :   rfsEntryIterator(other.rfs, other.entry, other.entryno) {  };

//     rfsEntryIterator &operator++()
//     {
//         entryno++;

//         if (entry && entryno > entry->entry_count)
//         {
//             entry = nullptr;
//             entryno = 0;
//         }

//         return *this;
//     };

//     rfsEntryIterator operator++(int)
//     {
//         return rfsEntryIterator(rfs, entry, entryno++);
//     };

//     bool operator== ( const rfsEntryIterator &rhs )
//     {
//         return (entry == rhs.entry) && (entryno == rhs.entryno);
//     };

//     bool operator!= ( const rfsEntryIterator &rhs )
//     {
//         return (entry != rhs.entry) || (entryno != rhs.entryno);
//     };

//     ramfsEntry *operator* ();

// };

// inline ramfsEntry *rfsEntryIterator::operator* ()
// {
//     auto *block   = rfs.getBlock(dir->block_id);
//     auto *entries = (int32_t*)(block->data);
//     return rfs.getEntry(entries[entryno]);
// };