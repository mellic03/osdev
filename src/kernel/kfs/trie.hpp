#pragma once
#include <stdint.h>
#include <idk_vector.hpp>


namespace KFS
{
    // struct TrieNode;
    class  Trie;
}




struct kfsEntry
{
private:

public:
    char      name[32];
    kfsEntry *parent;

    kfsEntry *children['z'-'0'][16];
    uint32_t  nchildren['z'-'0'];

    // idk::vector<idk::vector<kfsEntry*>> children;
    // kfsEntry **children['z'-'0'];
    // uint16_t   nchildren['z'-'0'];
    // uint16_t   capacity['z'-'0'];

    kfsEntry( kfsEntry *parent, const char *filename );
    kfsEntry *getChild( const char *filename );
    kfsEntry *giveChild( const char *filename );

    void print_path();
};




class KFS::Trie
{
private:
    static constexpr uint8_t ch_min = ' ';
    static constexpr uint8_t ch_max = '~';
    kfsEntry *m_root;
    kfsEntry *_insert( const char*, kfsEntry* );
    // kfsEntry *_find( const char*, TrieNode* );

public:
    Trie();

    kfsEntry *insert( const char* );
    kfsEntry *find( const char* );

};