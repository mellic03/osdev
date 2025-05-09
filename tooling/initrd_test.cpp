#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <cstring>
#include "../include/kernel/ramfs.hpp"



int main( int argc, char **argv )
{
    std::ifstream stream("ramdisk", std::ios::binary);

    rfsHeader header;
    RamFS *ramfs = new RamFS();
    size_t blockHeapSize;

    stream.read(reinterpret_cast<char*>(&header), sizeof(rfsHeader));
    stream.read(reinterpret_cast<char*>(ramfs), sizeof(RamFS));

    // std::cout << "header.block_count: " << header.block_count << "\n";
    blockHeapSize = header.block_count * header.block_size;
    ramfs->m_blockHeapData = (uint8_t*)std::malloc(blockHeapSize);
    stream.read(reinterpret_cast<char*>(ramfs->m_blockHeapData), blockHeapSize);

    std::cout << "ramfs->m_blockHeapSize: " << ramfs->m_blockHeapSize << "\n";

    return 0;
}

