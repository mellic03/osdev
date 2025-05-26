// #include <filesystem/ramfs.hpp>
// #include <kernel/kstring.h>
// #include <kernel/log.hpp>
// #include <kernel/bitmanip.hpp>

// #include <algorithm>
// #include <new>

// static RamFS global_rfs;

// void rfs::init( void *base )
// {
//     global_rfs = RamFS(base);
// }

// void rfs::walk()
// {
//     global_rfs.walk(global_rfs.m_root->entry_id);
// }


// rfsEntry *rfs::open( const char *path )
// {
//     return global_rfs.open(path);
// }

// size_t rfs::fwrite( rfsEntry *file, const uint8_t *src, size_t nbytes )
// {
//     return global_rfs.fwrite(file, src, nbytes);
// }

// size_t rfs::fread( rfsEntry *file, uint8_t *dst, size_t nbytes )
// {
//     return global_rfs.fread(file, dst, nbytes);
// }

// size_t rfs::ftell( rfsEntry *fh )
// {
//     return fh->cursor;
// }

// size_t rfs::fseek( rfsEntry *fh, size_t p )
// {
//     fh->cursor = p;
//     return fh->cursor;
// }

// size_t rfs::fsize( rfsEntry *fh )
// {
//     return global_rfs.fsize(fh);
// }


// size_t RamFS::fsize( rfsEntry *file )
// {
//     size_t size = 0;
//     int id = file->block_id;

//     while (id != -1)
//     {
//         auto *block = getBlock(id);
//         size += block->tail - block->base;
//         id = block->next_id;
//     }

//     return size;
// }






// RamFS::RamFS( void *base )
// {
//     m_header = (rfsHeader*)base;
//     m_base   = (uintptr_t)base;
//     m_end    = m_base + m_header->size;
//     loadDisk();
// }


// RamFS::RamFS( void *base, size_t size )
// {
//     m_base = (uintptr_t)base;
//     m_end  = m_base + size;
//     formatDisk();
// }



// void RamFS::loadDisk()
// {
//     m_header = (rfsHeader*)m_base;
//     auto &H = *m_header;

//     m_entryHeap   = (rfsEntry*)(m_base + H.entryHeapOffset);
//     m_entryBitmap = (idk_BitMap*)(m_base + H.entryBitmapOffset);

//     m_blockHeap   = (   uint8_t*)(m_base + H.blockHeapOffset);
//     m_blockBitmap = (idk_BitMap*)(m_base + H.blockBitmapOffset);

//     m_root        = (rfsEntry*)(m_base + H.rootOffset);

//     #ifndef KERNEL_TOOLING
//         syslog log("RamFS::loadDisk");
//         log("size:              %lu MB", H.size / (1024*1024));
//         log("blockSize:         %lu", H.blockSize);
//         log("rootOffset:        %lu", H.rootOffset);
//         log("entryBitmapOffset: %lu", H.entryBitmapOffset);
//         log("blockBitmapOffset: %lu", H.blockBitmapOffset);
//         log("entryHeapOffset:   %lu", H.entryHeapOffset);
//         log("blockHeapOffset:   %lu", H.blockHeapOffset);
//     #endif
// }


// void RamFS::formatDisk()
// {
//     m_header = (rfsHeader*)m_base;
//     *m_header = {
//         .size          = (size_t)(m_end - m_base),
//         .blockSize     = 64*1024,
//         .entryCapacity = 256,
//         .blockCapacity = 4*1024,
//     };

//     auto &H = *m_header;
//     size_t offset = sizeof(rfsHeader);

//     // m_entryBitmap
//     H.entryBitmapOffset = offset;
//     m_entryBitmap = (idk_BitMap*)(m_base+offset);
//     m_entryBitmap->nbits    = H.entryCapacity;
//     m_entryBitmap->setcount = 0;
//     m_entryBitmap->clear();
//     offset += sizeof(idk_BitMap) + m_entryBitmap->nbytes();

//     // m_blockBitmap
//     H.blockBitmapOffset = offset;
//     m_blockBitmap = (idk_BitMap*)(m_base+offset);
//     m_blockBitmap->nbits    = H.blockCapacity;
//     m_blockBitmap->setcount = 0;
//     m_blockBitmap->clear();
//     offset += sizeof(idk_BitMap) + m_blockBitmap->nbytes();

//     // m_entryHeap
//     H.entryHeapOffset = offset;
//     m_entryHeap = (rfsEntry*)(m_base+offset);
//     offset += H.entryCapacity * sizeof(rfsEntry);

//     // m_blockHeap
//     H.blockHeapOffset = offset;
//     m_blockHeap = (uint8_t*)(m_base+offset);
//     offset += H.blockCapacity * H.blockSize; // sizeof(rfsBlock);

//     // m_root
//     m_root = allocEntry(rfsEntry_Directory, "root");
//     m_header->rootOffset = (uintptr_t)m_root - m_base;

// }





// int32_t RamFS::allocBlock( rfsBlock *tail )
// {
//     int32_t block_id = allocID(*m_blockBitmap);
//     auto *block = getBlock(block_id);
//     *block = rfsBlock(m_header->blockSize);

//     if (tail)
//     {
//         tail->next_id = block_id;
//         block->idx    = tail->idx + 1;
//     }

//     return block_id;
// }


// rfsEntry *RamFS::allocEntry( uint32_t type, const char *name )
// {
//     int32_t entry_id = allocEntryID();
//     auto   *entry    = getEntry(entry_id);

//     *entry = {
//         .type        = type,
//         .flags       = 0,
//         // .size        = (uint32_t)(m_header->blockSize),
//         .cursor      = 0,
//         .entry_id    = entry_id,
//         .parent_id   = -1,
//         .block_id    = allocBlock(),
//         .entry_count = 0
//     };

//     memset(entry->name, '\0', 32);
//     strncpy(entry->name, name, 32);
//     appendBlock(entry);

//     if (type == rfsEntry_Directory)
//     {
//         auto *block   = getBlock(entry->block_id);
//         auto *entries = (int32_t*)(block->data);
//         for (int i=0; i<16; i++)
//         {
//             entries[i] = -1;
//         }
//     }

//     // std::print(
//     //     "[allocEntry] name=\"{}\", id={}, type={}\n",
//     //     name, entry_id, type
//     // );

//     return entry;
// }




// rfsEntry *RamFS::getEntry( int32_t entry_id )
// {
//     // assert((id >= 0));
//     return &(m_entryHeap[entry_id]);
// }

// rfsBlock *RamFS::getBlock( int32_t block_id )
// {
//     // assert((id >= 0));
//     return (rfsBlock*)(m_blockHeap + m_header->blockSize*block_id);
// }

// rfsBlock *RamFS::getLastBlock( int32_t block_id )
// {
//     rfsBlock *block = getBlock(block_id);
//     while (block->next_id != -1)
//         block = getBlock(block->next_id);
//     return block;
// }

// rfsBlock *RamFS::getIndexedBlock( rfsEntry *entry, int32_t idx )
// {
//     int block_id  = entry->block_id;
//     int block_idx = 0;

//     while (block_id != -1)
//     {
//         rfsBlock *block = getBlock(block_id);
    
//         if (block_idx == idx)
//             return block;
//         else
//             block_id = block->next_id;

//         block_idx += 1;
//     }

//     return nullptr;
// }

// rfsBlock *RamFS::appendBlock( rfsEntry *entry )
// {
//     // entry->size += m_header->blockSize;
//     auto *prev = getLastBlock(entry->block_id);
//     return getBlock(allocBlock(prev));
// }


// // void RamFS::addToDirectory( int32_t dir_id, int32_t entry_id )
// void RamFS::addToDirectory( rfsEntry *dir, rfsEntry *entry )
// {
//     auto *block = getBlock(dir->block_id);
//     auto *entries = (int32_t*)(block->data);
//     entries[dir->entry_count++] = entry->entry_id;
//     entry->parent_id = dir->entry_id;
// }


// size_t RamFS::block_write( rfsBlock *block, const uint8_t *src, size_t nbytes )
// {
//     size_t count = 0;
//     while ((count < nbytes) && (block->tail < block->end))
//     {
//         if (src) block->data[block->tail++] = *(src++);
//         else     block->data[block->tail++] = 0;
//         count++;
//     }
//     return count;
// }


// size_t RamFS::bwrite( rfsBlock *block, size_t pos, const uint8_t *src, size_t nbytes )
// {
//     size_t count = 0;

//     while (count < nbytes)
//     {
//         block->data[pos++] = *(src++);
//         count++;
    
//         if (pos >= block->end)
//         {
//             if (block->next_id == -1)
//                 block = getBlock(allocBlock(block));
//             else
//                 block = getBlock(block->next_id);
//             pos = 0;
//         }
//     }

//     return count;
// }


// size_t RamFS::fwrite( rfsEntry *file, const uint8_t *src, size_t nbytes )
// {
//     size_t count = 0;
//     auto  *block = getLastBlock(file->block_id);

//     while (count < nbytes)
//     {
//         size_t remaining = nbytes - count;
//         size_t written   = block_write(block, src, remaining);

//         if (src) src += written;
//         count += written;
//         file->cursor += written;

//         if (count < nbytes)
//         {
//             block->next_id = allocBlock();
//             block = getBlock(block->next_id);
//         }
//     }

//     return count;
// }

// size_t RamFS::fread( rfsEntry *file, uint8_t *dst, size_t nbytes )
// {
//     // int block_idx = file->cursor / m_header->blockSize;

//     size_t count = 0;
//     // size_t idx   = file->cursor / m_header->blockSize;
//     // size_t pos   = file->cursor % m_header->blockSize;
//     // auto  *block = getIndexedBlock(file, idx);
//     size_t pos   = 0;
//     auto  *block = getBlock(file->block_id);

//     while ((count < nbytes) && (pos < block->end))
//     {
//         *(dst++) = block->data[pos++];

//         // if (pos >= m_header->blockSize)
//         // {
//         //     if (block->next_id == -1)
//         //         block->next_id = allocBlock(block);

//         //     block = getBlock(block->next_id);
//         //     pos   = 0;
//         // }
//     }

//     return 0;
// }








// rfsEntry *RamFS::find_child( rfsEntry *dir, const char *name )
// {
//     if (dir->block_id == -1)
//     {
//         // std::cout << "[find_child] BRUH WTF\n";
//         // std::print(
//         //     "[find_child] parent=\"{}\", child=\"{}\", nEntries={}\n",
//         //     dir->name, name, dir->nEntries
//         // );

//         // exit(1);
//         return nullptr;
//     }

//     auto *block   = getBlock(dir->block_id);
//     auto *entries = (int32_t*)(block->data);

//     for (uint32_t i=0; i<dir->entry_count; i++)
//     {
//         if (entries[i] < 0)
//             return nullptr;

//         auto *entry = getEntry(entries[i]);
//         if (strcmp(entry->name, name) == 0)
//             return entry;
//     }

//     return nullptr;
// }




// rfsEntry *RamFS::open( const char *path )
// {
//     static char name[64];
//     memset(name, '\0', sizeof(name));

//     auto *A = skip_ch(path, ' ');
//           A = skip_ch(A,    '/');
//     auto *B = seek_ch(A,    '/');

//     rfsEntry *curr = m_root;

//     while (A < B)
//     {
//         strncpy(name, A, B-A);

//         if (curr->type != rfsEntry_Directory)
//         {
//             return nullptr;
//         }

//         rfsEntry *next = find_child(curr, name);

//         // File already exists
//         if ((next != nullptr) && (*B == '\0'))
//         {
//             return next;
//         }

//         // File needs to be created
//         else if (*B == '\0')
//         {
//             next = allocEntry(rfsEntry_File, name);
//             addToDirectory(curr, next);
//             return next;
//         }

//         else if (next == nullptr)
//         {
//             next = allocEntry(rfsEntry_Directory, name);
//             addToDirectory(curr, next);
//         }

//         curr = next;
//         A = skip_ch(B, '/');
//         B = seek_ch(A, '/');
//     }

//     // kassert("Should be unreachable", false);

//     return nullptr;
// }


// #ifndef KERNEL_TOOLING
//     #define INDENT for (int i=0; i<indent; i++) syslog::print(" ")
// #else
//     #define INDENT for (int i=0; i<indent; i++) std::print(" ")
// #endif


// void RamFS::walk( int32_t entry_id, int depth )
// {
//     static int indent = 0;

//     if (depth >= 4)
//     {
//         return;
//     }

//     auto *entry = getEntry(entry_id);
//     if (entry->type != rfsEntry_Directory)
//     {
//         INDENT;
//         #ifndef KERNEL_TOOLING
//             syslog::print("- %s (%u KB)\n", entry->name, fsize(entry)/1024);
//         #else
//             std::print("- {} ({} KB)\n", entry->name, fsize(entry)/1024);
//         #endif
//         return;
//     }

//     INDENT;

//     #ifndef KERNEL_TOOLING
//         syslog::print("/%s\n", entry->name);
//     #else
//         std::print("/{}\n", entry->name);
//     #endif


//     indent += 1;

//     auto *block = getBlock(entry->block_id);
//     auto *entries = (int32_t*)(block->data);

//     for (uint32_t i=0; i<entry->entry_count; i++)
//     {
//         indent += 1;
//         walk(entries[i], depth+1);
//         indent -= 1;
//     }

//     indent -= 1;
// }

// #undef INDENT

