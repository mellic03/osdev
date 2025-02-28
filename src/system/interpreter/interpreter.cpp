#include "./interpreter.hpp"
#include <iostream>




/*
    typename label

*/

#define SCAN_SRC( __code ) while (src) { __code; src++; }


int token_match( const char *src, const char *token )
{
    for (uint32_t i=0; i<len; i++)
    {
        if (src[i] != token[i])
        {
            return 0;
        }
    }

    return len;
}


int check_DTYPE_INT8( const char *src )
{
    SCAN_SRC
    (
        
    )
}


int check_typename( const char *src )
{
    SCAN_SRC
    (
        if (token_match(src, "int8"))
        {

        }
    )


    while (*src)
    {
        src++
    }
}



/**
 * @return offset to next ';' in src
 */
int check_typename_label( const char *src )
{
    while (*src)
    {
        std::cout << (*src) << " ";


        src++;
    }


    return 0;
}


int
ckInterpreter::Exec( const char *src )
{
    uint8_t stack[256];
    uint8_t *sp = &(stack[0]);

    while (*src)
    {


        src++;
    }
}

