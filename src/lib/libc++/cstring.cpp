// #include <cstring.hpp>


// size_t
// std::strlen( const char *str ) 
// {
// 	const char *base = str;
// 	const char *end  = str;

// 	while (*end)
// 	{
// 		end++;
// 	}

// 	return (size_t)(end - base - 1);
// }


// int
// std::strcmp( const char *lhs, const char *rhs )
// {
//     while (*lhs && *lhs == *rhs)
//     {
//         lhs++;
//         rhs++;
//     }

//     return (*lhs > *rhs) - (*rhs > *lhs);
// }




// void*
// std::memcpy( void *dst, const void *src, size_t n )
// {
//     uint8_t *pdst = (uint8_t *)dst;
//     const uint8_t *psrc = (const uint8_t *)src;

//     for (size_t i = 0; i < n; i++)
//     {
//         pdst[i] = psrc[i];
//     }

//     return dst;
// }



// void*
// std::memset( void *s, uint8_t c, size_t n )
// {
//     uint8_t *p = (uint8_t *)s;

//     for (size_t i = 0; i < n; i++)
//     {
//         p[i] = (uint8_t)c;
//     }

//     return s;
// }


// void*
// std::memmove( void *dest, const void *src, size_t n )
// {
//     uint8_t *pdest = (uint8_t *)dest;
//     const uint8_t *psrc = (const uint8_t *)src;

//     if (src > dest)
//     {
//         for (size_t i = 0; i < n; i++)
//         {
//             pdest[i] = psrc[i];
//         }
//     }

//     else if (src < dest)
//     {
//         for (size_t i = n; i > 0; i--)
//         {
//             pdest[i-1] = psrc[i-1];
//         }
//     }

//     return dest;
// }


// int
// std::memcmp( const void *s1, const void *s2, size_t n )
// {
//     const uint8_t *p1 = (const uint8_t *)s1;
//     const uint8_t *p2 = (const uint8_t *)s2;

//     for (size_t i = 0; i < n; i++) {
//         if (p1[i] != p2[i]) {
//             return p1[i] < p2[i] ? -1 : 1;
//         }
//     }

//     return 0;
// }



