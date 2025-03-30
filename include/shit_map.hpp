#pragma once
#include <ctype.h>
#include <vector>
#include <string>
#include <tuple>

namespace idk
{
    class shit_map;
}


class idk::shit_map
{
private:
    std::vector<std::vector<std::string>> table;

    void swap_pop( int i, int j )
    {
        auto tmp = table[i][j];
        table[i][j] = table[i].back();
        table[i].back() = tmp;
        table[i].pop_back();
    }


public:
    shit_map(): table('z'-'a') {  }

    std::pair<int, int>
    find( const std::string &str )
    {
        int i = tolower(str[0]) - 'a';
    
        for (int j=0; j<table[i].size(); j++)
        {
            if (strcmp(table[i][j].c_str(), str.c_str()) == 0)
            {
                return std::make_pair(i, j);
            }
        }

        return std::make_pair(-1, -1);
    }

    bool contains( const std::string &str )
    {
        auto p = find(str);
        return (p.first != -1) && (p.second != -1);
    }

    void insert( const std::string &str )
    {
        if (contains(str))
            return;
        int i = tolower(str[0]) - 'a';
        table[i].push_back(str);
    }

    void erase( const std::pair<int, int> &p )
    {
        int i = p.first;
        int j = p.second;

        if (i < table.size() && j < table[i].size())
        {
            swap_pop(i, j);
        }
    }

    // T &operator[] ( size_t i ) { return *(m_base+i); }
    // const T &operator[] ( size_t i ) const { return *(m_base+i); }

    // struct iterator;
    // iterator begin() { return iterator(m_base); };
    // iterator end()   { return iterator(m_top);  };
};



// template <typename T, size_t cap>
// struct idk::shit_map<T, cap>::iterator
// {
//     T *ptr;

//     iterator( T *p )                 : ptr(p)         {  };
//     iterator( const iterator &other ): ptr(other.ptr) {  };

//     iterator &operator++()
//     {
//         ptr++;
//         return *this;
//     };

//     iterator operator++(int)
//     {
//         return iterator(ptr++);
//     };

//     bool operator == ( const iterator &rhs ) { return ptr == rhs.ptr; };
//     bool operator != ( const iterator &rhs ) { return ptr != rhs.ptr; };
//     T &operator * () { return *ptr; };

// };



