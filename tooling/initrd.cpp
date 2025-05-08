#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <print>

#include "../include/kernel/ramfs.hpp"




int main( int argc, char **argv )
{
    namespace fs = std::filesystem;

    if (argc != 3)
    {
        std::cout << "Usage create_ramdisk [source_directory] [output_filepath]\n";
        return 1;
    }

    size_t MEGA = 1024*1024;
    size_t blockHeapSize = 64 * MEGA;

    RamFS *ramfs = new RamFS(
        new uint8_t[1024], 1024/8,
        new uint8_t[1024], 1024/8,
        new uint8_t[blockHeapSize], blockHeapSize
    );

    fs::path rootdir(argv[1]);
    for (auto entry: fs::directory_iterator(rootdir))
    {
        if (entry.is_directory())
        {
            continue;
        }

        std::cout << "entry: " << entry.path().c_str() << "\n";
        std::string filename = entry.path().filename().string();

        std::ifstream stream(entry.path().string(), std::ios::binary);
        size_t size = entry.file_size();

        void *buf = std::malloc(size);
        stream.read(reinterpret_cast<char*>(buf), size);
        ramfs->insert(filename.c_str(), buf, size);
        std::free(buf);
    
        stream.close();
    }

    RamFSHeader header = {
        .inode_count = 1024,
        .block_count = blockHeapSize / RamFS::BLOCK_SIZE,
        .block_size  = RamFS::BLOCK_SIZE,
        .padding     = 0
    };


    std::ofstream stream(argv[2], std::ios::binary);
    stream.write(reinterpret_cast<const char*>(&header), sizeof(RamFSHeader));
    stream.write(reinterpret_cast<const char*>(ramfs), sizeof(RamFS));
    stream.write(reinterpret_cast<const char*>(ramfs->m_blockHeapData), blockHeapSize);
    stream.close();

    return 0;
}

