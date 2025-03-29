#pragma once

#include <vector>
#include <kernel/memory.hpp>


namespace filesystem
{
    class path;
}


class filesystem::path
{
private:
    std::vector<char*> m_data;
    std::vector<char*> m_data2;
    char *m_name;
    char *m_ext;

public:
    path( const char *str );
    

};



