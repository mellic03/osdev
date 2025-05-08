#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <print>

#define KERNEL_TOOLING
#include "../kernel/include/driver/ramfs.hpp"
#include "../kernel/src/driver/ramfs.cpp"



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

        uint8_t *buf = new uint8_t[filesize];
        stream.read(reinterpret_cast<char*>(buf), filesize);
    
            // std::print("File \"{}\"\n", relpath.c_str());
            ramfsEntry *rfsEntry = rfs.open(relpath.c_str());
            // std::print("  Read success\n");
            rfs.fwrite(rfsEntry, buf, filesize);
            // std::print("  Write success\n");

        delete[] buf;
        stream.close();
    }

    std::print("size:              {}\n",  rfs.m_header->size);
    std::print("blockSize:         {}\n",  rfs.m_header->blockSize);
    std::print("rootOffset:        {}\n",  rfs.m_header->rootOffset);
    std::print("entryBitmapOffset: {}\n",  rfs.m_header->entryBitmapOffset);
    std::print("blockBitmapOffset: {}\n",  rfs.m_header->blockBitmapOffset);
    std::print("entryHeapOffset:   {}\n",  rfs.m_header->entryHeapOffset);
    std::print("blockHeapOffset:   {}\n",  rfs.m_header->blockHeapOffset);

    rfs.walk(rfs.m_root->entry_id);
    // std::print("[RamFS::RamFS] blockBitmap->setcount: {}\n", rfs.m_blockBitmap->m_setcount);
    // std::print("[RamFS::RamFS] blockCapacity: {}\n", rfs.m_header->blockCapacity);

    std::ofstream stream(argv[2], std::ios::binary);
    stream.write((const char*)(rfs.m_base), rfs.m_header->size);
    stream.close();

    return 0;
}

