#include <string>



std::string::string( const char *str )
{
    size_t len = strlen(str);
    reserve(2*len);
    strcpy(m_base, str);
    resize(len + 1);

}


