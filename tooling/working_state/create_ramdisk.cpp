#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <print>

// #include "../include/kernel/ramfs.hpp"
#include "ramfs.hpp"



int main( int argc, char **argv )
{
    namespace fs = std::filesystem;

    if (argc != 3)
    {
        std::cout << "Usage create_ramdisk [source_directory] [output_filepath]\n";
        return 1;
    }

    size_t MEGA = 1024*1024;
    RamFS rfs(malloc(64*MEGA), 64*MEGA);


    for (auto &entry: fs::recursive_directory_iterator(fs::path(argv[1])))
    {
        if (entry.is_directory())
        {
            continue;
        }

        fs::path relpath = fs::relative(entry.path(), fs::path(argv[1]));
        std::string filename = relpath.filename().string();
        std::ifstream stream(relpath.string(), std::ios::binary);
        size_t filesize = entry.file_size();

        void *buf = std::malloc(filesize);
        stream.read(reinterpret_cast<char*>(buf), filesize);

        std::print("File \"{}\"\n", relpath.c_str());

        ramfsEntry *rfsEntry = rfs.open(relpath.c_str());
        std::print("  Read success\n");

        rfs.file_write(rfsEntry, (const uint8_t*)buf, filesize);
        std::print("  Write success\n");

        std::free(buf);
        stream.close();
    }

    // rfs.walk(rfs.m_root->entry_id);
    // std::print("[RamFS::RamFS] blockBitmap->setcount: {}\n", rfs.m_blockBitmap->m_setcount);
    // std::print("[RamFS::RamFS] blockCapacity: {}\n", rfs.m_header->blockCapacity);

    std::ofstream stream(argv[2], std::ios::binary);
    stream.write((const char*)(rfs.m_header), rfs.m_header->totalSize);
    stream.close();

    return 0;
}

