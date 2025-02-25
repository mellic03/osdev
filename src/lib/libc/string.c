#include "string.h"


size_t strlen(const char *str) 
{
	const char *base = str;
	const char *end  = str;

	while (*end)
	{
		end++;
	}

	return (size_t)(end - base - 1);
}

