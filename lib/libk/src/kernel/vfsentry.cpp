#include <kernel/vfsentry.hpp>
#include <kernel/log.hpp>
// #include <kstring.h>
// #include <kmalloc.h>
#include <string.h>
#include <stdio.h>

static char m_buf[128];

vfsEntry::vfsEntry( vfsDirEntry *P, const std::string &fname )
:   parent(P),
    name(fname)
{


}




vfsFileEntry::vfsFileEntry( vfsDirEntry *P, const std::string &fname )
:   vfsEntry (P, fname),
    type     (0),
    addr     (nullptr),
    other    (nullptr),
    size     (0),
    stream   (nullptr, 0)
{
    m_is_dir = false;
}


vfsFileEntry::vfsFileEntry( vfsDirEntry *P, const std::string &fname,
                            uint8_t tp, void *ad, size_t sz )
:   vfsEntry (P, fname),
    type     (tp),
    addr     (ad),
    other    (nullptr),
    size     (sz),
    stream   (ad, sz)
{
    m_is_dir = false;

}



// vfsEntry *fpath_match( vfsEntry *entry, const char *path )
// {
//     if (entry->is_file()) // base case
//     {
//         if (strcmp(entry->get_path(), path) == 0)
//             return entry;
//         return nullptr;
//     }

//     for (vfsEntry *child: *((vfsDirEntry*)entry))
//     {
//         auto *fh = fpath_match(child, path);

//         if (fh != nullptr)
//         {
//             return fh;
//         }
//     }

//     return nullptr;
// }


// vfsEntry *filepath_match( const char *path )
// {
//     auto *entry = vfsDirEntry::rootdir;

//     for (vfsEntry *child: *entry)
//     {
//         auto *fh = fpath_match(child, path);

//         if (fh != nullptr)
//         {
//             return fh;
//         }
//     }

//     return nullptr;
// }




const char*
vfsDirEntry::get_path()
{
    memset(m_buf, '\0', sizeof(m_buf));
    char *top = m_buf;

    int  idx = 0;
    vfsDirEntry *nodes[32];
    vfsDirEntry *E = this;

    while (E) // != vfsEntry::rootdir)
    {
        nodes[idx++] = E;
        E = E->parent;
    }
    idx--;
    
    E = nodes[idx--];
    top += sprintf(top, "%s/", E->name.c_str());

    while (idx >= 0)
    {
        E = nodes[idx];
        top += sprintf(top, "%s/", E->name.c_str());
        idx--;
    }

    return m_buf;
}


const char*
vfsFileEntry::get_path()
{
    memset(m_buf, '\0', sizeof(m_buf));
    char *top = m_buf;

    top += sprintf(top, "%s", parent->get_path());
    top += sprintf(top, "%s", this->name.c_str());

    return m_buf;
}







