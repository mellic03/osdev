#include <driver/ramfs.hpp>
#include <kernel/kstring.h>
#include <kernel/log.hpp>
#include <kernel/bitmanip.hpp>

#include <algorithm>
#include <new>

static RamFS global_rfs;

void rfs::init( void *base )
{ global_rfs = RamFS(base); }

ramfsEntry *rfs::open( const char *path )
{ return global_rfs.open(path); }

size_t rfs::fwrite( ramfsEntry *file, const uint8_t *src, size_t nbytes )
{ return global_rfs.fwrite(file, src, nbytes); }

// size_t rfs::fread( ramfsEntry *file, uint8_t *dst, size_t nbytes );

size_t rfs::ftell( ramfsEntry *fh )
{ return fh->cursor; }

size_t rfs::fseek( ramfsEntry *fh, size_t p )
{ fh->cursor = p; return p; }



RamFS::RamFS( void *base )
{
    m_header = (ramfsHeader*)base;
    m_base   = (uintptr_t)base;
    m_end    = m_base + m_header->size;
    loadDisk();
}


RamFS::RamFS( void *base, size_t size )
{
    m_base = (uintptr_t)base;
    m_end  = m_base + size;
    formatDisk();
}



void RamFS::loadDisk()
{
    m_header = (ramfsHeader*)m_base;
    auto &H = *m_header;

    m_entryHeap   = (ramfsEntry*)(m_base + H.entryHeapOffset);
    m_entryBitmap = (idk_BitMap*)(m_base + H.entryBitmapOffset);

    m_blockHeap   = (   uint8_t*)(m_base + H.blockHeapOffset);
    m_blockBitmap = (idk_BitMap*)(m_base + H.blockBitmapOffset);

    m_root        = (ramfsEntry*)(m_base + H.rootOffset);

    #ifndef KERNEL_TOOLING
        syslog log("RamFS::loadDisk");
        log("size:              %lu MB", H.size / (1024*1024));
        log("blockSize:         %lu", H.blockSize);
        log("rootOffset:        %lu", H.rootOffset);
        log("entryBitmapOffset: %lu", H.entryBitmapOffset);
        log("blockBitmapOffset: %lu", H.blockBitmapOffset);
        log("entryHeapOffset:   %lu", H.entryHeapOffset);
        log("blockHeapOffset:   %lu", H.blockHeapOffset);
    #endif
}


void RamFS::formatDisk()
{
    m_header = (ramfsHeader*)m_base;
    *m_header = {
        .size          = (size_t)(m_end - m_base),
        .blockSize     = 4096,
        .entryCapacity = 128,
        .blockCapacity = 32*1024,
    };

    auto &H = *m_header;
    size_t offset = sizeof(ramfsHeader);

    // m_entryBitmap
    H.entryBitmapOffset = offset;
    m_entryBitmap = (idk_BitMap*)(m_base+offset);
    m_entryBitmap->nbits    = H.entryCapacity;
    m_entryBitmap->setcount = 0;
    m_entryBitmap->clear();
    offset += sizeof(idk_BitMap) + m_entryBitmap->nbytes();

    // m_blockBitmap
    H.blockBitmapOffset = offset;
    m_blockBitmap = (idk_BitMap*)(m_base+offset);
    m_blockBitmap->nbits    = H.blockCapacity;
    m_blockBitmap->setcount = 0;
    m_blockBitmap->clear();
    offset += sizeof(idk_BitMap) + m_blockBitmap->nbytes();

    // m_entryHeap
    H.entryHeapOffset = offset;
    m_entryHeap = (ramfsEntry*)(m_base+offset);
    offset += H.entryCapacity * sizeof(ramfsEntry);

    // m_blockHeap
    H.blockHeapOffset = offset;
    m_blockHeap = (uint8_t*)(m_base+offset);
    offset += H.blockCapacity * H.blockSize; // sizeof(ramfsBlock);

    // m_root
    m_root = allocEntry(ramfsEntry_Directory, "root");
    m_header->rootOffset = (uintptr_t)m_root - m_base;

    // #ifndef KERNEL_TOOLING
    //     syslog log("RamFS::formatDisk");
    //     log("rootOffset:        %lu", H.rootOffset);
    //     log("entryBitmapOffset: %lu", H.entryBitmapOffset);
    //     log("blockBitmapOffset: %lu", H.blockBitmapOffset);
    //     log("entryHeapOffset:   %lu", H.entryHeapOffset);
    //     log("blockHeapOffset:   %lu", H.blockHeapOffset);
    //     log("next:              %lu", offset);
    // #else
    //     std::print("rootOffset:        {}\n",  H.rootOffset);
    //     std::print("entryBitmapOffset: {}\n",  H.entryBitmapOffset);
    //     std::print("blockBitmapOffset: {}\n",  H.blockBitmapOffset);
    //     std::print("entryHeapOffset:   {}\n",  H.entryHeapOffset);
    //     std::print("blockHeapOffset:   {}\n",  H.blockHeapOffset);
    //     std::print("next:              {}\n",  offset);
    // #endif

    // assert((data < m_end));
}





int32_t RamFS::allocBlock( ramfsBlock *tail )
{
    int32_t block_id = allocID(*m_blockBitmap);
    auto *block = getBlock(block_id);
    *block = ramfsBlock(m_header->blockSize);

    if (tail)
    {
        tail->next_id = block_id;
        block->idx    = tail->idx + 1;
    }

    return block_id;
}


ramfsEntry *RamFS::allocEntry( uint32_t type, const char *name )
{
    int32_t entry_id = allocEntryID();
    auto   *entry    = getEntry(entry_id);

    *entry = {
        .type        = type,
        .flags       = 0,
        .size        = 0,
        .cursor      = 0,
        .entry_id    = entry_id,
        .parent_id   = -1,
        .block_id    = allocBlock(),
        .entry_count = 0
    };

    memset(entry->name, '\0', 32);
    strncpy(entry->name, name, 32);
    *(getBlock(entry->block_id)) = ramfsBlock(m_header->blockSize);

    if (type == ramfsEntry_Directory)
    {
        auto *block   = getBlock(entry->block_id);
        auto *entries = (int32_t*)(block->data);
        for (int i=0; i<16; i++)
        {
            entries[i] = -1;
        }
    }

    // std::print(
    //     "[allocEntry] name=\"{}\", id={}, type={}\n",
    //     name, entry_id, type
    // );

    return entry;
}




ramfsEntry *RamFS::getEntry( int32_t entry_id )
{
    // assert((id >= 0));
    return &(m_entryHeap[entry_id]);
}

ramfsBlock *RamFS::getBlock( int32_t block_id )
{
    // assert((id >= 0));
    return (ramfsBlock*)(m_blockHeap + m_header->blockSize*block_id);
}

ramfsBlock *RamFS::getLastBlock( int32_t block_id )
{
    ramfsBlock *block = getBlock(block_id);
    while (block->next_id != -1)
        block = getBlock(block->next_id);
    return block;
}


// void RamFS::addToDirectory( int32_t dir_id, int32_t entry_id )
void RamFS::addToDirectory( ramfsEntry *dir, ramfsEntry *entry )
{
    auto *block = getBlock(dir->block_id);
    auto *entries = (int32_t*)(block->data);
    entries[dir->entry_count++] = entry->entry_id;
    entry->parent_id = dir->entry_id;
}

// void RamFS::appendBlock( ramfsEntry *entry )
// {
//     auto *prev = getLastBlock(entry->block_id);
//     prev->next_id = allocBlock();

//     auto *next = getBlock(prev->next_id);
//     next->idx = prev->idx + 1;
// }



size_t RamFS::block_write( ramfsBlock *block, const uint8_t *src, size_t nbytes )
{
    size_t count = 0;
    while ((count < nbytes) && (block->tail < block->end))
    {
        block->data[block->tail++] = *(src++);
        count++;
    }
    return count;
}


size_t RamFS::fwrite( ramfsEntry *file, const uint8_t *src, size_t nbytes )
{
    if (file->block_id == -1)
    {
        file->block_id = allocBlock();
    }

    size_t count = 0;
    auto  *block = getLastBlock(file->block_id);

    while (count < nbytes)
    {
        size_t remaining = nbytes - count;
        size_t written   = block_write(block, src, remaining);

        src   += written;
        count += written;
        file->size += written;

        if (count < nbytes)
        {
            block->next_id = allocBlock();
            block = getBlock(block->next_id);
        }
    }

    return count;
}


// size_t RamFS::fread( ramfsEntry *file, void *dst, size_t nbytes )
// {
//     int block_idx = file->cursor / m_header->blockSize;
//     int block_ree = file->cursor % m_header->blockSize;

//     return 0;
// }









ramfsEntry *RamFS::find_child( ramfsEntry *dir, const char *name )
{
    if (dir->block_id == -1)
    {
        // std::cout << "[find_child] BRUH WTF\n";
        // std::print(
        //     "[find_child] parent=\"{}\", child=\"{}\", nEntries={}\n",
        //     dir->name, name, dir->nEntries
        // );

        // exit(1);
        return nullptr;
    }

    auto *block   = getBlock(dir->block_id);
    auto *entries = (int32_t*)(block->data);

    for (uint32_t i=0; i<dir->entry_count; i++)
    {
        if (entries[i] < 0)
            return nullptr;

        auto *entry = getEntry(entries[i]);
        if (strcmp(entry->name, name) == 0)
            return entry;
    }

    return nullptr;
}




ramfsEntry *RamFS::open( const char *path )
{
    static char name[64];
    auto *A = skip_ch(path, ' ');
          A = skip_ch(A,    '/');
    auto *B = seek_ch(A,    '/');

    ramfsEntry *curr = m_root;
    ramfsEntry *next = nullptr;

    while (A < B)
    {
        memset(name, '\0', sizeof(name));
        strncpy(name, A, B-A);

        if (curr->type != ramfsEntry_Directory)
        {
            return nullptr;
        }

        next = find_child(curr, name);

        // File already exists
        if ((next != nullptr) && (*B == '\0'))
        {
            return next;
        }

        // File needs to be created
        else if (*B == '\0')
        {
            next = allocEntry(ramfsEntry_File, name);
            addToDirectory(curr, next);
            return next;
        }

        else if (next == nullptr)
        {
            next = allocEntry(ramfsEntry_Directory, name);
            addToDirectory(curr, next);
        }

        curr = next;
        A = skip_ch(B, '/');
        B = seek_ch(A, '/');
    }

    // kassert("Should be unreachable", false);

    return nullptr;
}


#ifndef KERNEL_TOOLING
    #define INDENT for (int i=0; i<indent; i++) syslog::kprintf(" ")
#else
    #define INDENT for (int i=0; i<indent; i++) std::print(" ")
#endif


void RamFS::walk( int32_t entry_id, int depth )
{
    static int indent = 0;

    if (depth >= 3)
    {
        return;
    }

    auto *entry = getEntry(entry_id);
    if (entry->type != ramfsEntry_Directory)
    {
        INDENT;
        #ifndef KERNEL_TOOLING
            syslog::kprintf("- %s (%u KB)\n", entry->name, entry->size/1024);
        #else
            std::print("- {} ({} KB)\n", entry->name, entry->size/1024);
        #endif

        return;
    }

    INDENT;

    #ifndef KERNEL_TOOLING
        syslog::kprintf("/%s\n", entry->name);
    #else
        std::print("/{}\n", entry->name);
    #endif


    indent += 1;

    auto *block = getBlock(entry->block_id);
    auto *entries = (int32_t*)(block->data);

    for (uint32_t i=0; i<entry->entry_count; i++)
    {
        indent += 1;
        walk(entries[i], depth+1);
        indent -= 1;
    }

    indent -= 1;
}

#undef INDENT

