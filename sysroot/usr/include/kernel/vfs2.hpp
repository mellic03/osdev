#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <kmalloc.h>


enum vfs2Entry_: uint64_t
{
    vfs2Entry_Invalid = 0,
    vfs2Entry_Generic,
    vfs2Entry_Directory,

    vfs2Entry_RawFile,
    vfs2Entry_BlockDevice,
    vfs2Entry_CharDevice,
};

enum vfs2Flag_: uint64_t
{
    vfs2Flag_None    = 0,
    vfs2Flag_Symlink = 1<<0,
};



struct vfs2Header
{
    uint64_t type;
    uint64_t flags;
    char     name[32];
    size_t   size; // size of data section only
    uint8_t  *sof;
    uint8_t  *eof;
    uint8_t  *pos;

    vfs2Header( const char *nm, uint64_t tp, size_t sz, void *data );
};


struct vfs2Entry: vfs2Header
{
    static constexpr uint64_t vfsType = vfs2Entry_Generic;
    // uint8_t rest[];

    using vfs2Header::vfs2Header;

    vfs2Entry( const char *nm, uint64_t sz, void *p=nullptr )
    :   vfs2Header(nm, vfsType, sz, p) {  };
};



namespace vfs2
{
    extern vfs2Entry *root;
    void init();
    void walk( vfs2Entry *dir );

    vfs2Entry *resize( vfs2Entry*, uint64_t nbytes );
    vfs2Entry *find( const char *path );
    vfs2Entry *open( const char *path, uint64_t nbytes=0, void *data=nullptr );

    template <typename entry_type>
    entry_type *find( const char *path )
    {
        return  (entry_type*)(vfs2::find(path));
    }

    template <typename entry_type>
    entry_type *open( const char *path, uint64_t nbytes=0, void *data=nullptr )
    {
        return (entry_type*)(vfs2::open(path, sizeof(entry_type) + nbytes, data));
    }

}





struct vfs2DirEntry: vfs2Header
{
    static constexpr uint64_t vfsType = vfs2Entry_Directory;
    uint64_t   count;
    vfs2Entry **entries;

    using vfs2Header::vfs2Header;

    vfs2DirEntry( const char *nm, uint64_t sz, void *p=nullptr )
    :   vfs2Header(nm, vfsType, sz, p)
    {
        entries = (vfs2Entry**)sof;
    };
};


struct vfs2RawFile: vfs2Header
{
    static constexpr uint64_t vfsType = vfs2Entry_RawFile;
    // uint8_t rest[];

    vfs2RawFile( const char *nm, uint64_t sz, void *p )
    :   vfs2Header(nm, vfsType, sz, p) {  };

    size_t write( const void *src, size_t n );
    size_t write_str( const char *src );
};


struct vfs2BlockDevice: vfs2Header
{
    static constexpr uint64_t vfsType = vfs2Entry_BlockDevice;
    // uint8_t rest[];

    vfs2BlockDevice( const char *nm, uint64_t sz, void *p )
    :   vfs2Header(nm, vfsType, sz, p) {  };
};


struct vfs2CharDevice: vfs2Header
{
private:
    // std::mutex m_mutex;

public:
    static constexpr uint64_t vfsType = vfs2Entry_CharDevice;

    uint32_t readpos;
    uint32_t writepos;
    // uint8_t rest[];

    vfs2CharDevice( const char *nm, uint64_t sz, void *p )
    :   vfs2Header(nm, vfsType, sz, p),
        readpos(0), writepos(0) {  };

    size_t read( void *dstptr, size_t nbytes )
    {
        // m_mutex.lock();
        auto *dst = static_cast<uint8_t*>(dstptr);
        size_t count = 0;
    
        while ((count < nbytes) && (readpos != writepos))
        {
            *(dst++) = sof[readpos++];
            readpos %= this->size;

            count++;
        }
    
        // m_mutex.unlock();
        return count;
    }
    
    
    size_t write( const void *srcptr, size_t nbytes )
    {
        // m_mutex.lock();
        auto *src = static_cast<const uint8_t*>(srcptr);
        size_t count = 0;
    
        while (count < nbytes)
        {
            auto next_write = (writepos+1) % this->size;
            if (next_write == readpos)
            {
                break;
            }

            sof[writepos] = *(src++);
            writepos = next_write;
            count++;
        }
    
        // m_mutex.unlock();
        return count;
    }

};
