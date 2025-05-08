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

    if (argc != 2)
    {
        std::cout << "Usage read_ramdisk [input_filepath]\n";
        return 1;
    }

    std::ifstream stream(argv[1], std::ios::binary);
    size_t filesize = fs::file_size(fs::path(argv[1]));

    void *buf = malloc(filesize);
    stream.read((char*)(buf), filesize);
    stream.close();

    RamFS rfs(buf);


    return 0;
}

