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
:   vfsEntry(P, fname)
{
    m_is_dir = false;
}


vfsFileEntry::vfsFileEntry( vfsDirEntry *P, const std::string &fname,
                            uint8_t tp, void *ad, size_t sz )
:   vfsEntry (P, fname),
    type     (tp),
    addr     (ad),
    size     (sz),
    stream   (ad, sz)
{
    m_is_dir = false;

}





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







