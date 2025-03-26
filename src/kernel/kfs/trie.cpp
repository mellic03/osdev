#include "trie.hpp"
#include "../driver/serial.hpp"
#include <string.h>
#include <kstring.h>
#include <kmalloc.h>
#include <stdio.h>


// KFS::TrieNode::TrieNode( char c, const char *label )
// :   idx(c),
//     lbl(nullptr)
// {
//     if (label)
//     {
//         lbl = new char[strlen(label)+1];
//         strcpy(lbl, label);
//     }

//     memset(ch, 0, sizeof(ch));
// }





// KFS::TrieNode::~TrieNode()
// {
//     if (lbl)
//     {
//         delete[] lbl;
//     }

// }


kfsEntry::kfsEntry( kfsEntry *P, const char *filename )
{
    memset(name, 0, sizeof(name));
    strcpy(name, filename);
    parent = P;

    memset(children,  0, sizeof(children));
    memset(nchildren, 0, sizeof(nchildren));

    // children = idk::vector<idk::vector<kfsEntry*>>('z'-'0');
    //     // 'z'-'0', idk::vector<kfsEntry*>(0)
    // // );

    // for (int i=0; i<children.size(); i++)
    // {
    //     children[i] = idk::vector<kfsEntry*>(0);
    // }

    // SYSLOG("chilen.size: %d", children.size());
}


kfsEntry*
kfsEntry::getChild( const char *filename )
{
    int idx = filename[0] - '0';

    if (nchildren[idx] == 0)
    {
        return nullptr;
    }

    for (int i=0; i<nchildren[idx]; i++)
    {
        kfsEntry *E = children[idx][i];

        if (strcmp(E->name, filename) == 0)
        {
            return E;
        }
    }

    return nullptr;
}



kfsEntry*
kfsEntry::giveChild( const char *filename )
{
    int idx = filename[0] - '0';

    // if (getChild(filename) != nullptr)
    // {
    //     return nullptr;
    // }

    // kfsEntry *E = nullptr;

    // if (nchildren[idx] < 16)
    // {
    //     int i = nchildren[idx];
    //     children[idx][i] = new kfsEntry(this, filename);
    //     E = children[idx][i];
    // }

    auto *child = new kfsEntry(this, filename);

    int count = nchildren[idx];
    children[idx][count++] = child;

    return child;
}


void
kfsEntry::print_path()
{
    char *buf[32];
    int  idx = 0;
    kfsEntry *E = this;

    while (E)
    {
        buf[idx++] = E->name;
        E = E->parent;
    }
    idx--;

    while (idx >= 0)
    {
        printf("/%s", buf[idx]);
        idx--;
    }

    printf("\n");

}





KFS::Trie::Trie()
:   m_root(new kfsEntry(nullptr, "root"))
{
    
}


// "/home/michael/Desktop/shady.exec";


kfsEntry*
KFS::Trie::_insert( const char *path, kfsEntry *e )
{
    return nullptr;
    // static char buf[32];

    // auto *A = path;
    // auto *B = seek_ch(A+1, '/');
    // memcpy(buf, A, B-A);

    // if (*s == '\0')
    // {

    // }

    // char min_ch = ' ';
    // char max_ch = '~';

    // if (e->children[*S] == nullptr)
    // {
    //     e->children[*S] = new kfsEntry(*S, S);
    //     e->children[*S].

    //     S += 1;
    // }
}


kfsEntry*
KFS::Trie::insert( const char *path )
{
    if (m_root == nullptr)
    {
        m_root = new kfsEntry(nullptr, "root");
    }

    kfsEntry *E = m_root;
    static char name[32];

    while (*path && E)
    {
        auto *A = skip_ch(path, '/');
        auto *B = seek_ch(A+1, '/');
        memcpy(name, A, B-A);
        name[B-A] = '\0';

        auto *child = E->getChild(name);

        if (!child)
        {
            child = E->giveChild(name);
        }

        E = child;

        path = B;
    }

    printf("created file: ");
    E->print_path();

    return E;
}






// kfsEntry*
// KFS::Trie::_find( const char *path, kfsEntry *node )
// {

// }




kfsEntry*
KFS::Trie::find( const char *path )
{
    kfsEntry *E = m_root;
    static char name[32];

    while (*path && E)
    {
        auto *A = skip_ch(path, '/');
        auto *B = seek_ch(A+1, '/');
        memcpy(name, A, B-A);
        name[B-A] = '\0';

        auto *child = E->getChild(name);

        if (!child)
        {
            return nullptr;
        }

        E = child;
        path = B;
    }

    return E;
}

