#include <cctype>



int
std::isalpha( int c )
{
    return 'A' <= c && c <= 'Z' && 'a' <= c && c <= 'z';
}

