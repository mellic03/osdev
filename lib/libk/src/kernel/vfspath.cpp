#include <kernel/vfspath.hpp>
#include <kernel/log.hpp>
#include <kstring.h>
#include <string.h>



// int tokenize( char **buf, int limit, const char *str, char delim )
// {
//     int len = strlen(str);
//     int idx = 0;
//     auto *A = str;
//     auto *B = A;

//     while ((A < str+len) && (idx < limit))
//     {
//         B = seek_ch(A+1, delim);
//         strncpy(buf[idx++], A, B-A);
//         A = B+1;
//     }

//     return idx;
// }


std::vector<char*>&
tokenize( std::vector<char*> &out, const char *str, char delim )
{
    int len = strlen(str);
    auto *A = str;
    auto *B = A;

    while (A < str+len)
    {
        B = seek_ch(A+1, delim);
        out.push_back(new char[B-A+1]);
        strncpy(out.back(), A, B-A);
        A = B+1;
    }

    return out;
}




static std::vector<char*> temp;
static char zero = '\0';

filesystem::path::path( const char *str )
:   m_data(0),
    m_name(&zero),
    m_ext (&zero)
{
    syslog log("filesystem::path::path");
    log("input: \"%s\"", str);

    m_data = tokenize(m_data, str, '/');

    if (count_ch(m_data.back(), '.') == 1)
    {
        m_data2 = tokenize(m_data2, m_data.back(), '.');
        delete[] m_data.back();
        m_data.pop_back();

        m_name = new char[strlen(m_data2[0])+1];
        m_ext  = new char[strlen(m_data2[1])+1];
        strcpy(m_name, m_data2[0]);
        strcpy(m_ext,  m_data2[1]);

    }

    for (char *name: m_data)
    {
        log("dir:  \"%s\"", name);
    }

    log("name: \"%s\"", m_name);
    log("ext:  \"%s\"", m_ext);

}