#include "vfsentry.hpp"
#include "../log/log.hpp"
#include <string.h>
#include <kstring.h>
#include <kmalloc.h>
#include <stdio.h>



kfsEntry::kfsEntry( vfsDirEntry *P, const char *fname )
:   parent(P)
{
    memset(name, 0, sizeof(name));
    strncpy(name, fname, sizeof(name)-1);
}


kfsEntry*
vfsDirEntry::getChild( const char *fname )
{
    for (kfsEntry *E: children)
    {
        if (strcmp(E->name, fname) == 0)
        {
            return E;
        }
    }

    return nullptr;
}


// kfsEntry*
// vfsDirEntry::giveChild( kfsEntry *entry )
// {
//     // int idx = fname[0] - '0';
//     // auto *child = new kfsEntry(this, filename);

//     // int count = nchildren[idx];
//     // children[idx][count++] = child;

//     children.push_back(entry);

//     return children.back();
// }



const char*
vfsDirEntry::get_path()
{
    // syslog log("vfsDirEntry::print_path");
    memset(m_buf, 0, sizeof(m_buf));
    char *top = m_buf;

    int  idx = 0;
    vfsDirEntry *nodes[32];
    vfsDirEntry *prev = this;
    vfsDirEntry *E = this;

    while (E)
    {
        // log("nodes[%d] = \"%s\"", idx, E->name);
        nodes[idx++] = E;
        prev = E;
        E = E->parent;
    }
    idx--;
    
    E = nodes[idx--];
    top += sprintf(top, "%s/", E->name);

    while (idx >= 0)
    {
        E = nodes[idx];
        // log("nodes[%d] = \"%s\"", idx, E->name);
        top += sprintf(top, "%s/", E->name);
        idx--;
    }

    return m_buf;
}


const char*
vfsFileEntry::get_path()
{
    char  buf[128];
    char *top = buf;

    top += sprintf(top, "%s", parent->get_path());
    top += sprintf(top, "%s", this->name);

    return buf;
}







