#include <string.h>
#include <stdlib.h>

char *strdup( const char *src )
{
    size_t len = strlen(src);
    char *dst = (char*)malloc(len*sizeof(char));
    return strcpy(dst, src);
}
