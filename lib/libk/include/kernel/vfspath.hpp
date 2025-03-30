#pragma once

#include <kernel/memory.hpp>
#include <vector>
#include <string>


namespace fs
{
    class path;
    class directorypath;
    class filepath;
}


class fs::path
{
private:
public:
    std::vector<std::string> m_sep;
    std::string m_dirname;
    std::string m_filename;

     path( const char *str );
    ~path();

};


class fs::directorypath: public fs::path
{
private:
public:
    directorypath( const char *str );
    // ~directorypath();

};


class fs::filepath: public fs::path
{
private:
public:
    filepath( const char *str );
    // ~filepath();

};



