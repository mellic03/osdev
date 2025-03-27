#pragma once

#include <kfs/kfs.hpp>
#include "vfsentry.hpp"


namespace KFS
{
    void   init( uintptr_t sys );
    KFile *KFile_create( size_t size, void (*)(KFile*) );

    vfsFileEntry *insertFile( const char* );
    vfsFileEntry *findFile( const char* );

    vfsDirEntry *insertDirectory( const char* );
    vfsDirEntry *findDirectory( const char* );

}


namespace KFS
{
    class Trie
    {
    private:
        static constexpr uint8_t ch_min = ' ';
        static constexpr uint8_t ch_max = '~';
        vfsDirEntry *m_root;
        inline static char m_buf0[128];
        inline static char m_buf1[128];
    
    public:
        Trie();
        vfsFileEntry *insertFile( const char* );
        vfsFileEntry *findFile( const char* );

        vfsDirEntry *insertDirectory( const char* );
        vfsDirEntry *findDirectory( const char* );
    
    };
}



