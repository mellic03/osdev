#pragma once
#include "kfstream.hpp"
#include <static_vector.hpp>
#include <string.h>

struct vfsEntry;
struct vfsDirEntry;
struct vfsFileEntry;


struct vfsEntry
{
protected:
    bool m_is_dir;

public:
    char name[128];
    vfsDirEntry *parent;

    vfsEntry( vfsDirEntry *P, const char *fname );
    virtual const char *get_path() = 0;

    bool is_dir()  { return  m_is_dir; };
    bool is_file() { return !m_is_dir; };

};


struct vfsDirEntry: public vfsEntry
{
    idk::static_vector<vfsEntry*, 16> children;

    vfsDirEntry( vfsDirEntry *P, const char *fname )
    :   vfsEntry(P, fname),
        children(0, nullptr)
    {
        m_is_dir = true;
    }

    const char *get_path() override;
    bool hasChild( const char *name ) { return getChild(name) != nullptr; }

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
    size_t    size;
    kfstream  stream;

    vfsFileEntry( vfsDirEntry *P, const char *fname );
    vfsFileEntry( vfsDirEntry *P, const char *fname, uint8_t type, void *addr, size_t size );

    const char *get_path() override;

};


