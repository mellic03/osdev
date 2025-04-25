#pragma once
#include "kfstream.hpp"
#include <string>
#include <static_vector.hpp>

struct vfsEntry;
struct vfsDirEntry;
struct vfsFileEntry;


enum vfsFileStatus_
{
    vfsFileStatus_Invalid = 0,
    vfsFileStatus_Good,
    vfsFileStatus_Dirty,
};


enum vfsFileFlag_
{
    vfsFileFlag_None       = 1<<0,
    vfsFileFlag_Virtual    = 1<<1,
    vfsFileFlag_Stream     = 1<<2,
    vfsFileFlag_Text       = 1<<3,
    vfsFileFlag_Executable = 1<<4,
    vfsFileFlag_MaxBit     = 4
};



struct vfsEntry
{
protected:
    bool m_is_dir;

public:
    static vfsDirEntry *rootdir;

    vfsDirEntry *parent;
    char         name[256];

    vfsEntry( vfsDirEntry *P, const char *fname );
    virtual const char *get_path() = 0;

    bool is_dir()  { return  m_is_dir; };
    bool is_file() { return !m_is_dir; };

};


struct vfsDirEntry: public vfsEntry
{
    idk::static_vector<vfsEntry*, 32> children;

    vfsDirEntry( vfsDirEntry *P, const char *fname )
    :   vfsEntry(P, fname),
        children(0, nullptr)
    {
        m_is_dir = true;
    }

    const char *get_path() override;
    bool hasChild( const char *fname )
    { return getChild(fname) != nullptr; }

    template <typename T = vfsEntry>
    T *getChild( const char *fname )
    {
        for (vfsEntry *E: children)
            if (strcmp(E->name, fname) == 0)
                return static_cast<T*>(E);
        return nullptr;
    }

    template <typename T, typename... Args>
    T *giveChild( const char *fname, Args... args )
    {
        T *entry = new T(this, fname, args...);
        children.push_back(static_cast<vfsEntry*>(entry));
        return entry;
    }

    auto begin() { return children.begin(); };
    auto end()   { return children.end();  };
};


struct vfsFileEntry: public vfsEntry
{
    uint8_t   type;
    uint32_t  flags = 0;
    void     *addr;
    void     *other;
    size_t    size;
    kfstream  stream;

    vfsFileEntry( vfsDirEntry *P, const char *fname );
    vfsFileEntry( vfsDirEntry *P, const char *fname, uint8_t type, void *addr, size_t size );

    const char *get_path() override;

};


