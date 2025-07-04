// #pragma once

// #include <stdarg.h>
// #include <stdbool.h>
// #include <stddef.h>
// #include <stdint.h>
// #include <bitmap2.hpp>
// #include <string.h>
// #include <ipc/pipe.hpp>


// class RamFS;

// enum rfsEntry_: uint32_t
// {
//     rfsEntry_Invalid = 0,
//     rfsEntry_Directory,
//     rfsEntry_File,
// };


// enum rfsFlag_: uint32_t
// {
//     rfsFlag_None      = 0,
//     rfsFlag_Symlink   = 1<<0,
//     rfsFlag_ReadCstm  = 1<<8,
//     rfsFlag_WriteCstm = 1<<9,
// };



// struct rfsBlock
// {
//     int32_t  idx;
//     int32_t  next_id;
//     size_t   base;
//     size_t   tail;
//     size_t   end;
//     uint8_t  data[1];

//     rfsBlock( size_t block_size )
//     {
//         idx     = 0;
//         next_id = -1;
//         base    = 0;
//         tail    = 0;
//         end     = block_size - offsetof(rfsBlock, data);
//     }
// };


// // struct rfsINode
// // {
// //     uint32_t nblocks;
// //     int32_t  blocks[32];

// //     rfsINode()
// //     {
// //         for (int i=0; i<32; i++)
// //             blocks[i] = -1;
// //         nblocks = 0;
// //     }
// // };


// struct rfsEntry
// {
//     uint32_t type;
//     uint32_t flags;
//     size_t   (*rcstm)(rfsEntry*, void*, size_t);
//     size_t   (*wcstm)(rfsEntry*, const void*, size_t);
//     void     *impl;
//     size_t   cursor;
//     char     name[32];
//     int32_t  entry_id;
//     int32_t  parent_id;
//     int32_t  block_id;
//     uint32_t entry_count;
// };


// struct rfsHeader
// {
//     size_t size;
//     size_t blockSize;

//     size_t entryCapacity;
//     size_t blockCapacity;

//     size_t entryBitmapOffset;
//     size_t blockBitmapOffset;
    
//     size_t entryHeapOffset;
//     size_t blockHeapOffset;

//     size_t rootOffset;
// };



// class RamFS
// {
// private:
//     template <typename bitmap_type>
//     int32_t allocID( bitmap_type &bitmap )
//     {
//         for (size_t i=0; i<bitmap.nbits; i++)
//         {
//             if (bitmap.is_unset(i))
//             {
//                 bitmap.set(i);
//                 return i;
//             }
//         }
//         return -1;
//     }
//     int32_t allocEntryID() { return allocID(*m_entryBitmap); }
//     int32_t allocBlock( rfsBlock *tail=nullptr );
//     rfsEntry *find_child( rfsEntry*, const char *name );
//     size_t block_write( rfsBlock *block, const uint8_t *src, size_t nbytes );
//     size_t bwrite( rfsBlock *block, size_t pos, const uint8_t *src, size_t nbytes );


// public:
//     rfsHeader   *m_header;
//     uintptr_t    m_base;
//     uintptr_t    m_end;

//     idk_BitMap  *m_entryBitmap;
//     idk_BitMap  *m_blockBitmap;

//     rfsEntry    *m_entryHeap;
//     uint8_t     *m_blockHeap;

//     rfsEntry    *m_root;

//     RamFS() {  };
//     RamFS( void *base );
//     RamFS( void *base, size_t size );

//     void      formatDisk();
//     void      loadDisk();
//     void      addToDirectory( rfsEntry *dir, rfsEntry *entry );

//     rfsEntry *allocEntry( uint32_t type, const char *name );
//     rfsEntry *getEntry( int32_t entry_id );
//     rfsBlock *getBlock( int32_t block_id );
//     rfsBlock *getLastBlock( int32_t block_id );
//     rfsBlock *getIndexedBlock( rfsEntry *entry, int32_t idx );
//     rfsBlock *appendBlock( rfsEntry *entry  );

//     rfsEntry *open( const char *path );
//     void      walk( int32_t entry_id, int depth=0 );
//     size_t    fwrite( rfsEntry *file, const void *src, size_t nbytes );
//     size_t    fread ( rfsEntry *file, void *dst, size_t nbytes );
//     // void      fprintf( rfsEntry *file, const uint8_t *src, size_t nbytes );
//     size_t    fsize( rfsEntry *fh );


//     // rfsEntryIterator begin() { return rfsEntryIterator(*this, m_root, 0);  };
//     // rfsEntryIterator end()   { return rfsEntryIterator(*this, nullptr, 0); };
// };



// namespace rfs
// {
//     // void init( void *base );
//     void init();
//     void walk();

//     void *open( const char *path );

//     size_t    fwrite( void *file, const void *src, size_t nbytes );
//     size_t    fread ( void *file, void *dst, size_t nbytes );
//     size_t    ftell( rfsEntry *fh );
//     size_t    fseek( rfsEntry *fh, size_t p );
//     size_t    fsize( rfsEntry *fh );

// }




// // struct rfsEntryIterator
// // {
// //     RamFS &rfs;
// //     rfsEntry *entry;
// //     uint32_t entryno;

// //     rfsEntryIterator( RamFS &fs, rfsEntry *E, uint32_t eno )
// //     :   rfs(fs), entry(E), entryno(eno)
// //     {
// //         if (entry && entryno > entry->entry_count)
// //         {
// //             entry = nullptr;
// //             entryno = 0;
// //         }
// //     };

// //     rfsEntryIterator( const rfsEntryIterator &other )
// //     :   rfsEntryIterator(other.rfs, other.entry, other.entryno) {  };

// //     rfsEntryIterator &operator++()
// //     {
// //         entryno++;

// //         if (entry && entryno > entry->entry_count)
// //         {
// //             entry = nullptr;
// //             entryno = 0;
// //         }

// //         return *this;
// //     };

// //     rfsEntryIterator operator++(int)
// //     {
// //         return rfsEntryIterator(rfs, entry, entryno++);
// //     };

// //     bool operator== ( const rfsEntryIterator &rhs )
// //     {
// //         return (entry == rhs.entry) && (entryno == rhs.entryno);
// //     };

// //     bool operator!= ( const rfsEntryIterator &rhs )
// //     {
// //         return (entry != rhs.entry) || (entryno != rhs.entryno);
// //     };

// //     rfsEntry *operator* ();

// // };

// // inline rfsEntry *rfsEntryIterator::operator* ()
// // {
// //     auto *block   = rfs.getBlock(dir->block_id);
// //     auto *entries = (int32_t*)(block->data);
// //     return rfs.getEntry(entries[entryno]);
// // };