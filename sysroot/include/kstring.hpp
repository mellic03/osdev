#pragma once

#include "kstring.h"
#include <vector>

namespace kstring
{
    std::vector<char*> &tokenize( const char *str, char delim );
}
