#include <filesystem/vfs.hpp>
#include <kernel/kstring.h>
#include <kernel/log.hpp>
#include <kassert.h>
#include <util/bitmap.hpp>
#include <algorithm>
#include <tuple>
#include <new>

static vfsNode     *vfs_Root;
static std::mutex   vfs_Lock;

static idk::BitMapAllocator<vfsInode, 1024>   vfs_Inodes;
static idk::BitMapAllocator<vfsBlock, 2*4096> vfs_Blocks;


vfsNode::vfsNode( const char *name, uint32_t type )
:   m_type(type), m_flags(0),
    m_impl(nullptr), m_readfn(nullptr), m_writefn(nullptr),
    m_inode(-1)
{
    memset(m_name, '\0', sizeof(m_name));
    strncpy(m_name, name, NAME_MAXLEN);

    if (type == vfsNode_Directory)
    {
        m_inode = vfs_Inodes.allocIdx();
        auto *inode = vfs_Inodes.get(m_inode);
              inode->idx = 0;
              inode->block = vfs_Blocks.allocIdx();

        auto *block = vfs_Blocks.get(inode->block);
        memset(block, 0, sizeof(vfsDirListing));
    }
}



void vfs::init()
{
    syslog log("vfs::init");
    vfs_Blocks.clear();
    vfs_Inodes.clear();
    vfs_Root = new vfsNode("root", vfsNode_Directory);

}





static vfsNode *vfs_Recurse( vfsNode *dir, bool (*cond)(vfsNode*) )
{
    int64_t inode_id = dir->m_inode;

    while (inode_id != -1)
    {
        auto *inode   = vfs_Inodes.get(inode_id);
        auto *block   = vfs_Blocks.get(inode->block);
        auto *subdirs = (vfsDirListing*)block;

        for (size_t i=0; i<subdirs->count; i++)
        {
            vfsNode *child = subdirs->entries + i;
            if (cond(child))
                return child;
            else
                return vfs_Recurse(child, cond);
        }

        inode_id = inode->next;
    }

    return nullptr;
}



// static vfsNode *vfs_Find( vfsNode *dir, bool (*cond)(vfsNode*) )
static vfsNode *vfs_Find( vfsNode *dir, const char *name )
{
    kassert(dir->m_inode != -1);

    int64_t inode_id = dir->m_inode;
    while (inode_id != -1)
    {
        auto *inode   = vfs_Inodes.get(inode_id);
        auto *block   = vfs_Blocks.get(inode->block);
        auto *subdirs = (vfsDirListing*)block;

        for (size_t i=0; i<subdirs->count; i++)
        {
            vfsNode *child = subdirs->entries + i;
            if (strcmp(child->m_name, name) == 0)
                return child;
        }

        inode_id = inode->next;
    }

    return nullptr;
}



static vfsNode *vfs_CopyToDir( vfsNode *dir, const vfsNode &child )
{
    kassert(dir->m_inode != -1);

    int64_t inode_id = dir->m_inode;
    while (inode_id != -1)
    {
        auto *inode   = vfs_Inodes.get(inode_id);
        auto *block   = vfs_Blocks.get(inode->block);
        auto *subdirs = (vfsDirListing*)block;

        subdirs->entries[subdirs->count] = child;
        return &subdirs->entries[subdirs->count++];

        // if block full, continue to next inode
        inode_id = inode->next;
    }

    return nullptr;
}



vfsNode *vfs::open( const char *path )
{
    std::lock_guard lock(vfs_Lock);

    char name[64];
    memset(name, '\0', sizeof(name));

    auto *A = skip_ch(path, ' ');
          A = skip_ch(A,    '/');
    auto *B = seek_ch(A,    '/');

    vfsNode *curr = vfs_Root;

    while (A < B)
    {
        strncpy(name, A, B-A);

        if (curr->m_type != vfsNode_Directory)
        {
            return nullptr;
        }

        vfsNode *next = vfs_Find(curr, name);
        // vfsNode *next = vfs_Find(curr, [](vfsNode *N) {
        //     return (strcmp(N->m_name, name) == 0);
        // });

        // File already exists
        if ((next != nullptr) && (*B == '\0'))
        {
            return next;
        }

        // File needs to be created
        else if (*B == '\0')
        {
            return vfs_CopyToDir(curr, vfsNode(name, vfsNode_File));
        }

        else if (next == nullptr)
        {
            next = vfs_CopyToDir(curr, vfsNode(name, vfsNode_Directory));
        }

        curr = next;
        A = skip_ch(B, '/');
        B = seek_ch(A, '/');
    }

    // kassert("Should be unreachable", false);

    return nullptr;
}


static vfsInode *inodeAppend( vfsInode *inode )
{
    kassert(inode->next == -1);

    vfsInode *prev = inode;

    inode->next = vfs_Inodes.allocIdx();
    inode = vfs_Inodes.get(inode->next);
    inode->idx = prev->idx + 1;
    inode->block = vfs_Blocks.allocIdx();

    return inode;
}


static const uint8_t *inodeWrite( vfsInode *inode, const uint8_t *src, size_t &nbytes )
{
    kassert(inode->block != -1);

    auto *block = (uint8_t*)vfs_Blocks.get(inode->block);
    uint8_t *dst = block; // + inode->size;
    uint8_t *end = block + vfs::BLOCK_SIZE;

    if (dst+nbytes > end)
    {
        size_t free_space = vfs::BLOCK_SIZE - inode->size;
        memcpy(dst, src, free_space);
        // inode->size += free_space;
        src        += free_space;
        nbytes     -= free_space;
    }

    else
    {
        memcpy(dst, src, nbytes);
        // inode->size += nbytes;
        src        += nbytes;
        nbytes      = 0;
    }

    return src;
}


static uint8_t *inodeRead( vfsInode *inode, uint8_t *dst, size_t &nbytes )
{
    kassert(inode->block != -1);

    auto *block = vfs_Blocks.get(inode->block);
    uint8_t *src = block->data; // + inode->size;
    uint8_t *end = block->data + vfs::BLOCK_SIZE;

    if (src+nbytes > end)
    {
        size_t free_space = vfs::BLOCK_SIZE - inode->size;
        memcpy(dst, src, free_space);
        // inode->size += free_space;
        dst        += free_space;
        nbytes     -= free_space;
    }

    else
    {
        memcpy(dst, src, nbytes);
        // inode->size += nbytes;
        dst        += nbytes;
        nbytes      = 0;
    }

    return dst;
}


size_t vfs::write( vfsNode *fh, const void *srcbuf, size_t, size_t nbytes )
{
    std::lock_guard lock(vfs_Lock);

    if (fh->m_writefn)
    {
        return fh->m_writefn(fh, srcbuf, nbytes);
    }

    if (fh->m_inode == -1)
    {
        fh->m_inode = vfs_Inodes.allocIdx();
        auto *inode = vfs_Inodes.get(fh->m_inode);
              inode->idx = 0;
              inode->block = vfs_Blocks.allocIdx();
    }

    auto *src = (const uint8_t*)srcbuf;
    auto *end = src + nbytes;
    auto *inode = vfs_Inodes.get(fh->m_inode);
    // size_t &cursor = handle->cursor;
    // size_t idx     = cursor / vfs::BLOCK_SIZE;

    // while (inode->idx != idx)
    // {
    //     if (inode->next == -1)
    //         inode = inodeAppend(inode);
    //     inode = vfs_Inodes.get(inode->next);
    // }

    while (src < end)
    {
        src = inodeWrite(inode, src, nbytes);
        if (inode->next == -1)
            inodeAppend(inode);
        inode = vfs_Inodes.get(inode->next);
    }

    return src - (uint8_t*)srcbuf;
}



size_t vfs::read( vfsNode *fh, void *dstbuf, size_t, size_t nbytes )
{
    std::lock_guard lock(vfs_Lock);

    if (fh->m_readfn)
        return fh->m_readfn(fh, dstbuf, nbytes);
    if (fh->m_inode == -1)
        return 0;
    if (vfs_Inodes.get(fh->m_inode)->block == -1)
        return 0;

    auto *dst = (uint8_t*)dstbuf;
    auto *end = dst + nbytes;
    auto *inode = vfs_Inodes.get(fh->m_inode);
    // size_t &cursor = handle->cursor;
    // size_t idx     = cursor / vfs::BLOCK_SIZE;

    // while (inode->idx != idx)
    // {
    //     if (inode->next == -1)
    //         return 0;
    //     inode = vfs_Inodes.get(inode->next);
    // }

    while (dst < end)
    {
        dst = inodeRead(inode, dst, nbytes);
        if (inode->next == -1)
            inodeAppend(inode);
        inode = vfs_Inodes.get(inode->next);
    }

    return dst - (uint8_t*)dstbuf;
}


size_t vfs::tell( vfsNode* )
{
    return 0;
    // return fh->cursor;
}


int vfs::seek( vfsNode*, int64_t, uint8_t )
{
    return 1;
}


// size_t vfs::size( vfsNode *fh )
// {
//     return 0;
// }



#define INDENT for (int i=0; i<indent; i++) syslog::print(" ")
static void vfs_Print( vfsNode *node, int depth )
{
    using namespace vfs;
    static int indent = 8;

    if (depth > 8)
        return;

    if (node->m_type != vfsNode_Directory)
    {
        INDENT;
        // syslog::print("- %s (%u KB)\n", node->name, fsize(node)/1024);
        syslog::print("- %s\n", node->m_name);
        return;
    }

    INDENT;
    syslog::print("/%s\n", node->m_name);


    indent += 1;

    auto *inode   = vfs_Inodes.get(node->m_inode);
    auto *block   = vfs_Blocks.get(inode->block);
    auto *subdirs = (vfsDirListing*)block;

    for (size_t i=0; i<subdirs->count; i++)
    {
        indent += 1;
        vfs_Print(subdirs->entries+i, depth+1);
        indent -= 1;
    }

    indent -= 1;
}
#undef INDENT

void vfs::print()
{
    syslog log("vfs::print");
    vfs_Print(vfs_Root, 0);
}





static void vfs_Walk( vfsNode *node, int depth, bool (*cond)(vfsNode*, int) )
{
    using namespace vfs;

    if (!cond(node, depth))
        return;

    if (node->m_type != vfsNode_Directory)
        return;

    int64_t inode_id = node->m_inode;
    while (inode_id != -1)
    {
        auto *inode   = vfs_Inodes.get(node->m_inode);
        auto *block   = vfs_Blocks.get(inode->block);
        auto *subdirs = (vfsDirListing*)block;
        for (size_t i=0; i<subdirs->count; i++)
            vfs_Walk(subdirs->entries+i, depth+1, cond);

        inode_id = inode->next;
    }

}


void vfs::walk( bool (*cond)(vfsNode*, int) )
{
    syslog log("vfs::walk");
    vfs_Walk(vfs_Root, 0, cond);
}


