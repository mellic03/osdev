#include <string.h>


// int memcmp( const void *s1, const void *s2, size_t n )
// {
//     const uint8_t *p1 = (const uint8_t *)s1;
//     const uint8_t *p2 = (const uint8_t *)s2;

//     for (size_t i=0; i<n; i++)
//     {
//         if (p1[i] != p2[i])
//         {
//             return p1[i] < p2[i] ? -1 : 1;
//         }
//     }

//     return 0;
// }

int memcmp(const void *str1, const void *str2, size_t count)
{
    const unsigned char *s1 = (const unsigned char*)str1;
    const unsigned char *s2 = (const unsigned char*)str2;

    while (count-- > 0)
    {
        if (*s1++ != *s2++)
            return s1[-1] < s2[-1] ? -1 : 1;
    }
    return 0;
}