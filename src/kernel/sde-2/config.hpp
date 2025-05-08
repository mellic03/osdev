#pragma once

#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kdef.h>
#include <kmalloc.h>


struct sdeConfigFileEntry
{
    char     label[16];
    size_t   nbytes;
    uint64_t data[];
};


struct sdeConfigFile
{
    size_t entry_count;
    sdeConfigFileEntry entries[];
};



void sdeConfigFile_create( const char *label )
{
    sdeConfigFile file = *(sdeConfigFile*)kmalloc(4096);
    // file.entries[]
}


void reee()
{
    sdeConfigFile *file;
    
    // file->entries
}

