// #pragma once
// #include "bitmap.hpp"
// #include <algorithm>


// struct RamFSHeader
// {
//     size_t inode_count;
//     size_t block_count;
//     size_t block_size;
//     size_t padding;
// };


// struct RamFS
// {
// public:
//     static constexpr size_t BLOCK_SIZE = 1024;

//     struct INode
//     {
//         size_t  size;
//         int32_t block_id;
//         int32_t next_id;
//     };

//     struct File
//     {
//         size_t  size;
//         char    name[32];
//         int32_t node_id;
//     };

//     idk::BitMap<1024>  m_inodeBitmap;
//     idk::BitMap<1024>  m_blockBitmap;

//     INode        m_inodes[1024];
//     uint8_t     *m_blockHeapData;
//     size_t       m_blockHeapSize;


//     int allocINode( size_t size=0 )
//     {
//         for (size_t i=0; i<m_inodeBitmap.size(); i++)
//         {
//             if (m_inodeBitmap.is_unset(i))
//             {
//                 m_inodeBitmap.set(i);
//                 m_inodes[i] = {
//                     .size     = size,
//                     .block_id = allocBlock(),
//                     .next_id  = -1
//                 };
//                 return i;
//             }
//         }
    
//         return -1;
//     }

//     int allocBlock()
//     {
//         for (size_t i=0; i<m_blockBitmap.size(); i++)
//         {
//             if (m_blockBitmap.is_unset(i))
//             {
//                 m_blockBitmap.set(i);
//                 return i;
//             }
//         }

//         return -1;
//     }

//     INode   *getNode  ( int id ) { return &(m_inodes[id]); }
//     uint8_t *getBlock ( int id ) { return m_blockHeapData + BLOCK_SIZE*id; }

//     size_t inode_write( INode *node, const uint8_t *srcbuf, size_t nbytes )
//     {
//         if (node->block_id == -1)
//         {
//             node->block_id = allocBlock();
//         }

//         uint8_t *dstbuf = getBlock(node->block_id);
//         uint8_t *dstend = dstbuf + nbytes;
//         uint8_t *dst    = dstbuf;

//         while (dst < dstend)
//         {
//             *(dst++) = *(srcbuf++);
//         }

//         return dst - dstbuf;
//     }

//     size_t file_write( File &file, const uint8_t *srcbuf, size_t nbytes )
//     {
//         if (file.node_id == -1)
//         {
//             file.size    = 0;
//             file.node_id = allocINode(BLOCK_SIZE);
//         }

//         auto *src    = srcbuf;
//         auto *srcend = srcbuf + nbytes;

//         INode *node = getNode(file.node_id);

//         while (src < srcend)
//         {
//             size_t written   = srcbuf - src;
//             size_t remaining = nbytes - written;
//             size_t blocksize = std::min(BLOCK_SIZE, remaining);

//             src += inode_write(node, src, blocksize);
    
//             if (src < srcend)
//             {
//                 node->next_id = allocINode(BLOCK_SIZE);
//                 node = getNode(node->next_id);
//             }
//         }

//         return src - srcbuf;
//     }


// public:

//     size_t insert( const char *name, const void *src, size_t nbytes )
//     {
//         File file = {
//             .size    = 0,
//             .node_id = -1
//         };
//         strncpy(file.name, name, 32);
//         return file_write(file, (const uint8_t*)src, nbytes);
//     }

//     RamFS( uint8_t *inodeBitmapData, size_t inodeBitmapSize,
//            uint8_t *blockBitmapData, size_t blockBitmapSize,
//            uint8_t *blockHeapData,   size_t blockHeapSize )
//     // :   m_inodeBitmap(inodeBitmapData, inodeBitmapSize),
//         // m_blockBitmap(blockBitmapData, blockBitmapSize),
//     :
//         m_blockHeapData(blockHeapData), // ((uint8_t*)malloc(64*1024*1024)),
//         m_blockHeapSize(blockHeapSize) // (64*1024*1024)
//     {

//     };

//     RamFS()
//     : RamFS(nullptr, 0, nullptr, 0, nullptr, 0) {  }
// };

