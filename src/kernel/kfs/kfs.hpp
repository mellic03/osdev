#pragma once

#include <kfs/kfs.hpp>
#include "trie.hpp"


namespace KFS
{
    void   init();
    KFile *KFile_create( size_t size, void (*)(KFile*) );
    
}


