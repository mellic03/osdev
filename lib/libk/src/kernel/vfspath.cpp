#include <kernel/vfspath.hpp>
#include <kernel/log.hpp>
#include <kstring.hpp>
#include <string.h>


static std::vector<std::string>&
tokenize( std::vector<std::string> &out, const char *str, char delim )
{
    // syslog log("tokenize");

    out.clear();

    int len = strlen(str);
    auto *A = str;
    auto *B = A;

    char buf[64];

    while (A < str+len)
    {
        B = seek_ch(A+1, delim);
        strncpy(buf, A, B-A);
        // log("buf: %s", buf);   
        out.push_back(std::string(A, B-A));
        A = B+1;
    }

    return out;
}




fs::path::path( const char *str )
{
    if (*str == '/')
        str++;
    m_sep = tokenize(m_sep, str, '/');
}


fs::path::~path()
{

}


fs::directorypath::directorypath( const char *str )
:   fs::path(str)
{
    // syslog log("fs::directorypath::directorypath");

    for (auto &name: m_sep)
        m_dirname += name + "/";

    // log("m_dirname: \"%s\"", m_dirname.c_str());
}


fs::filepath::filepath( const char *str )
:   fs::path(str)
{
    // syslog log("fs::filepath::filepath");

    if (m_sep.size() == 1)
    {
        m_dirname  = "./";
        m_filename = m_sep[0];
        return;
    }

    m_dirname = m_sep[0] + "/";
    for (int i=1; i<m_sep.size()-1; i++)
        m_dirname += m_sep[i] + "/";

    m_filename = m_sep.back();

    // log("m_dirname: \"%s\"", m_dirname.c_str());
    // log("m_filename: \"%s\"", m_filename.c_str());
}

