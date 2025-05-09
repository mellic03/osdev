#include "ramfs.hpp"
#include "../kernel/include/kernel/kstring.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <print>
#include <cassert>



RamFS::RamFS( void *base )
{
    m_header = (rfsHeader*)base;
    m_base   = (uintptr_t)base;
    m_end    = m_base + m_header->totalSize;

    const rfsHeader &H = *m_header;

    m_entryBitmap = (entryBitmap_type*)(m_base + H.entryBitmapOffset);
    m_blockBitmap = (blockBitmap_type*)(m_base + H.blockBitmapOffset);
    m_entryHeap   = (rfsEntry*)(m_base + H.entryHeapOffset);
    m_blockHeap   = (uint8_t*   )(m_base + H.blockHeapOffset);
    m_root        = (rfsEntry*)(m_base + H.rootOffset);

    // std::print("[RamFS::RamFS] blockCapacity: {}\n", m_header->blockCapacity);
    // std::print("[RamFS::RamFS] m_blockBitmap->m_setcount: {}\n", m_blockBitmap->m_setcount);
}


RamFS::RamFS( void *base, size_t size )
{
    m_base = (uintptr_t)base;
    m_end  = m_base + size;
    formatDisk();
}



void RamFS::formatDisk()
{
    // memset((void*)m_base, 0, m_end-m_base);
    size_t offset = 0;

    m_header = (rfsHeader*)m_base;
    *m_header = {
        .totalSize     = (size_t)(m_end - m_base),
        .blockSize     = RFS_BLOCK_SIZE,
        .entryCapacity = RamFS::rfsEntryCapacity,
        .blockCapacity = RamFS::rfsBlockCapacity
    };
    // data += sizeof(rfsHeader);
    offset += sizeof(rfsHeader);

    // m_entryBitmap
    m_header->entryBitmapOffset = offset;
    m_entryBitmap = (entryBitmap_type*)(m_base+offset);
    *m_entryBitmap = entryBitmap_type();
    m_entryBitmap->clear();
    offset += sizeof(entryBitmap_type);

    // m_blockBitMap
    m_header->blockBitmapOffset = offset;
    m_blockBitmap = (blockBitmap_type*)(m_base+offset);
    *m_blockBitmap = blockBitmap_type();
    m_blockBitmap->clear();
    offset += sizeof(blockBitmap_type);

    // m_entryHeap
    m_header->entryHeapOffset = offset;
    m_entryHeap = (rfsEntry*)(m_base+offset);
    offset += m_header->entryCapacity * sizeof(rfsEntry);

    // m_blockHeap
    m_header->blockHeapOffset = offset;
    m_blockHeap = (uint8_t*)(m_base+offset);
    offset += m_header->blockCapacity * m_header->blockSize;

    // m_root
    m_root = allocEntry(rfsEntry_Directory, "root");
    m_header->rootOffset = (uintptr_t)m_root - m_base;

    // assert((data < m_end));
}





int32_t RamFS::allocBlock()
{
    int32_t block_id = allocID(*m_blockBitmap);

    // if (block_id < 0)
    //     std::print("[allocBlock] m_blockBitmap.m_setcount: {}\n", m_blockBitmap->m_setcount);
    // assert((block_id >= 0));

    *(getBlock(block_id)) = rfsBlock();
    return block_id;
}


rfsEntry *RamFS::allocEntry( uint32_t type, const char *name )
{
    int32_t entry_id = allocEntryID();
    auto   *entry    = getEntry(entry_id);

    *entry = {
        .type      = type,
        .flags     = 0,
        .entry_id  = entry_id,
        .parent_id = -1,
        .block_id  = allocBlock(),
        .nEntries  = 0
    };

    memset(entry->name, '\0', 32);
    strncpy(entry->name, name, 32);

    *(getBlock(entry->block_id)) = rfsBlock();

    if (type == rfsEntry_Directory)
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




rfsEntry *RamFS::getEntry( int32_t id )
{
    // assert((id >= 0));
    return &(m_entryHeap[id]);
}

rfsBlock *RamFS::getBlock( int32_t id )
{
    // assert((id >= 0));
    return (rfsBlock*)(m_blockHeap + sizeof(rfsBlock)*id);
}


// void RamFS::addToDirectory( int32_t dir_id, int32_t entry_id )
void RamFS::addToDirectory( rfsEntry *dir, rfsEntry *entry )
{
    // auto *dir   = (rfsEntry*)getEntry(dir_id);
    // auto *entry = (rfsEntry*)getEntry(entry_id);
    auto *block = getBlock(dir->block_id);
    auto *entries = (int32_t*)(block->data);
    entries[dir->nEntries++] = entry->entry_id;
    entry->parent_id = dir->entry_id;
}


// void RamFS::printAbsolutePath( rfsEntry *entry )
// {
//     std::vector<std::string> names;
//     names.push_back(std::string(entry->name));

//     while (entry->parent_id != -1)
//     {
//         entry = getEntry(entry->parent_id);
//         if (entry == m_root)
//             break;
//         names.push_back(std::string(entry->name));
//     }

//     for (int i=names.size()-1; i>=0; i--)
//         std::cout << " / \"" << names[i] << "\"";
//     std::cout << std::endl;
// }








size_t RamFS::block_write( rfsBlock *block, const uint8_t *src, size_t nbytes )
{
    size_t count = 0;

    while ((count < nbytes) && (block->tail < block->end))
    {
        *(block->tail++) = *(src++);
        count++;
    }

    return count;
}


size_t RamFS::file_write( rfsEntry *file, const uint8_t *src, size_t nbytes )
{
    if (file->block_id == -1)
    {
        file->block_id = allocBlock();
    }

    size_t count = 0;
    auto  *block = getBlock(file->block_id);

    while (count < nbytes)
    {
        size_t remaining = nbytes - count;
        size_t written   = block_write(block, src, remaining);

        src   += written;
        count += written;

        if (count < nbytes)
        {
            block->next_id = allocBlock();
            block = getBlock(block->next_id);
        }
    }

    return count;
}









rfsEntry *RamFS::find_child( rfsEntry *dir, const char *name )
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

    for (uint32_t i=0; i<dir->nEntries; i++)
    {
        if (entries[i] < 0)
            return nullptr;

        auto *entry = getEntry(entries[i]);
        if (strcmp(entry->name, name) == 0)
            return entry;
    }

    return nullptr;
}




#include <iostream>

rfsEntry *RamFS::open( const char *path )
{
    // std::print("[RamFS::open] path=\"{}\"\n", path);

    auto *A = skip_ch(path, ' ');
          A = skip_ch(A,    '/');
    auto *B = seek_ch(A,    '/');

    static char name[64];
    memset(name, '\0', sizeof(name));

    rfsEntry *curr = m_root;

    
    while (A < B)
    {
        strncpy(name, A, B-A);
        // log("-----------------");
        // log("A:     %lu", A - path);
        // log("B:     %lu", B - path);
        // log("name: \"%s\"", name);
        // log("curr->type: %lu", curr->type);
        // std::cout << " / " << name;

        if (curr->type != rfsEntry_Directory)
        {
            return nullptr;
        }

        rfsEntry *next = find_child(curr, name);

        // File already exists
        if ((next != nullptr) && (*B == '\0'))
        {
            return next;
        }

        // File needs to be created
        else if (*B == '\0')
        {
            next = allocEntry(rfsEntry_File, name);
            addToDirectory(curr, next);
            return next;
        }

        else if (next == nullptr)
        {
            next = allocEntry(rfsEntry_Directory, name);
            addToDirectory(curr, next);
        }

        curr = next;
        A = skip_ch(B, '/');
        B = seek_ch(A, '/');
    }

    // kassert("Should be unreachable", false);

    return nullptr;
}



// #define INDENT for (int i=0; i<indent; i++) std::cout << " "
#define INDENT

void RamFS::walk( int32_t entry_id, int depth )
{
    static int indent = 0;

    if (depth >= 3)
    {
        return;
    }

    if (entry_id < 0)
    {
        std::cout << "GAY\n";
        exit(1);
    }

    auto *entry = getEntry(entry_id);

    if (entry->type != rfsEntry_Directory)
    {
        INDENT;
        // std::print("-{}\n", entry->name);
        return;
    }

    INDENT;
    // std::print("/{}\n", entry->name);
    indent += 1;

    auto *block = getBlock(entry->block_id);
    auto *entries = (int32_t*)(block->data);

    for (int32_t i=0; i<entry->nEntries; i++)
    {
        indent += 1;
        INDENT;
        // std::print("- entry_id={}\n", entries[i]);
        walk(entries[i], depth+1);
        indent -= 1;
    }

    indent -= 1;
}




