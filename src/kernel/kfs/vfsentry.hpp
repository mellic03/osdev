#pragma once

#include <kfs/kfs.hpp>
#include <idk_vector.hpp>

struct vfsDirEntry;


struct kfsEntry
{
protected:
    inline static char m_buf[128];
    bool is_dir;

public:
    char name[64];
    vfsDirEntry *parent;

    kfsEntry( vfsDirEntry *P, const char *fname );
    virtual ~kfsEntry() = default;
    virtual const char *get_path() = 0;

    bool is_directory() { return  is_dir; };
    bool is_file()      { return !is_dir; };

};


struct vfsDirEntry: public kfsEntry
{
    idk::vector<kfsEntry*> children;

    vfsDirEntry( vfsDirEntry *P, const char *fname )
    :   kfsEntry(P, fname)
    {
        is_dir = true;
    }

    virtual const char *get_path() final;

    bool      hasChild( const char *name ) { return getChild(name) != nullptr; }
    kfsEntry *getChild( const char *name );

    template <typename T>
    kfsEntry *giveChild( const char *name )
    {
        children.push_back(dynamic_cast<kfsEntry*>(new T(this, name)));
        return children.back();
    }

    auto begin() { return children.begin(); };
    auto end()   { return children.end();  };
};


struct vfsFileEntry: public kfsEntry
{
    uint8_t *base;
    uint8_t *top;
    uint8_t *eof;

    vfsFileEntry( vfsDirEntry *P, const char *fname )
    :   kfsEntry(P, fname)
    {
        is_dir = false;
    }

    virtual const char *get_path() final;

};

