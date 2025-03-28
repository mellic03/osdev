#pragma once

#include <kernel/memory.hpp>
#include <kfs/kfs.hpp>
#include <kinplace/inplace_vector.hpp>
#include <string.h>

struct vfsDirEntry;


enum vfsFileType_: uint8_t
{
    vfsFileType_DUMMY,
    vfsFileType_RAW,
    vfsFileType_FONT,
    vfsFileType_EXECUTABLE,
    vfsFileType_STREAM,
};

inline const char *vfsFileTypeStr( uint8_t type )
{
    switch (type)
    {
        default:                        return "INVALID_TYPE";
        case vfsFileType_DUMMY:         return "vfsFileType_DUMMY";
        case vfsFileType_RAW:           return "vfsFileType_RAW";
        case vfsFileType_FONT:          return "vfsFileType_FONT";
        case vfsFileType_EXECUTABLE:    return "vfsFileType_EXECUTABLE";
        case vfsFileType_STREAM:        return "vfsFileType_STREAM";
    }
};


struct kfsEntry
{
protected:
    bool m_is_dir;

public:
    char name[128];
    vfsDirEntry *parent;

    kfsEntry( vfsDirEntry *P, const char *fname );
    // virtual ~kfsEntry() = default;
    virtual const char *get_path() = 0;

    bool is_dir()  { return  m_is_dir; };
    bool is_file() { return !m_is_dir; };

};


struct vfsDirEntry: public kfsEntry
{
    kfsEntry *m_cbuf[16];
    idk::inplace_vector<kfsEntry*> children;

    vfsDirEntry( vfsDirEntry *P, const char *fname )
    :   kfsEntry(P, fname),
        children(m_cbuf, 16)
    {
        m_is_dir = true;
    }

    const char *get_path() override;
    bool hasChild( const char *name ) { return getChild(name) != nullptr; }

    template <typename T = kfsEntry>
    T *getChild( const char *fname )
    {
        for (kfsEntry *E: children)
            if (strcmp(E->name, fname) == 0)
                return static_cast<T*>(E);
        return nullptr;
    }


    template <typename T>
    T *giveChild( const char *fname )
    {
        T *entry = new T(this, fname);
        children.push_back(static_cast<kfsEntry*>(entry));
        return entry;
    }

    // template <typename T>
    // T *giveChild( const char *fname, uint8_t tp, void *ad, size_t sz )
    // {
    //     T *entry = new T(this, fname, tp, ad, sz);
    //     children.push_back(static_cast<kfsEntry*>(entry));
    //     return entry;
    // }

    auto begin() { return children.begin(); };
    auto end()   { return children.end();  };
};


struct vfsFileEntry: public kfsEntry
{
    uint8_t  type;
    void    *addr;
    size_t   size;

    vfsFileEntry( vfsDirEntry *P, const char *fname )
    :   kfsEntry(P, fname) // , vfsFileType_RAW, 0, 0)
    {
        m_is_dir = false;
    }

    // vfsFileEntry( vfsDirEntry *P, const char *fn, uint8_t tp, void *ad, size_t sz )
    // :   kfsEntry(P, fn), type(tp), addr(ad), size(sz)
    // {
    //     m_is_dir = false;
    // }

    const char *get_path() override;

};

