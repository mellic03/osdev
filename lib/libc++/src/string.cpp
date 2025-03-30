#include <string>

void
std::string::_realloc( size_t cap )
{
    size_t prevsize = size();
    char *bigga = new char[cap];

    if (m_base)
    {
        memcpy(bigga, m_base, prevsize);
        bigga[prevsize] = '\0';
        delete[] m_base;
    }

    m_cap  = cap;
    m_base = bigga;
    m_top  = m_base + prevsize;
    m_end  = m_base + m_cap;
}





std::string::string()
:   m_cap  (0),
    m_base (nullptr),
    m_top  (nullptr),
    m_end  (nullptr)
{

}


std::string::~string()
{
    if (m_base)
    {
        delete[] m_base;
    }
}


std::string::string( const char *str )
:   m_cap  (std::min(strlen(str), (size_t)64)),
    m_base (new char[m_cap]),
    m_top  (m_base + strlen(str)),
    m_end  (m_base + m_cap)
{
    strcpy(m_base, str);
    // strncpy(m_base, str, size());
    *m_top = '\0';
}



std::string::string( const char *str, size_t len )
:   m_cap  (std::min(2*len, (size_t)64)),
    m_base (new char[m_cap]),
    m_top  (m_base + len),
    m_end  (m_base + m_cap)
{
    strncpy(m_base, str, len);
    *m_top = '\0';
}


std::string::string( const std::string &rhs )
:   m_cap  (rhs.m_cap),
    m_base (new char[m_cap]),
    m_top  (m_base + rhs.size()),
    m_end  (m_base + m_cap)
{
    strncpy(m_base, rhs.m_base, rhs.size());
    *m_top = '\0';
}


std::string::string( std::string &&rhs ) noexcept
:   m_cap  (rhs.m_cap),
    m_base (rhs.m_base),
    m_top  (rhs.m_top),
    m_end  (rhs.m_end)
{
    rhs.m_cap  = 0;
    rhs.m_base = nullptr;
    rhs.m_top  = nullptr;
    rhs.m_end  = nullptr;
}



void
std::string::push_back( char ch )
{
    if (m_cap == 0)
        _realloc(64);
    else if (m_top+2 >= m_end)
        _realloc(2*m_cap);

    *(m_top++) = ch;
    *m_top = '\0';
}


void
std::string::pop_back()
{
    *m_top = '\0';
    m_top -= 1;
}


std::string&
std::string::operator=( const std::string &rhs )
{
    if (this == &rhs)
    {
        return *this;
    }

    // m_cap and m_end set by _realloc
    _realloc(rhs.m_cap);
    memcpy(m_base, rhs.m_base, rhs.size());
    m_top = m_base + rhs.size();

    return *this;
}

std::string&
std::string::operator=( std::string &&rhs )
{
    if (this == &rhs)
    {
        return *this;
    }

    m_cap  = rhs.m_cap;  rhs.m_cap  = 0;
    m_base = rhs.m_base; rhs.m_base = nullptr;
    m_top  = rhs.m_top;  rhs.m_top  = nullptr;
    m_end  = rhs.m_end;  rhs.m_end  = nullptr;


    return *this;
}





std::string&
std::string::operator=( const char *str )
{
    *this = std::string(str);
    return *this;
}








std::string&
std::string::operator+=( const std::string &rhs )
{
    auto *rtop = rhs.m_base;

    while (*rtop)
    {
        push_back(*(rtop++));
    }

    return *this;
}


std::string
std::string::operator+( const std::string &rhs )
{
    std::string lhs(m_base);
    lhs += rhs;
    return lhs;
}


bool
std::string::operator==( const std::string &rhs )
{
    return strcmp(c_str(), rhs.c_str()) == 0;
}

bool
std::string::operator==( const std::string &rhs ) const
{
    return strcmp(c_str(), rhs.c_str()) == 0;
}





