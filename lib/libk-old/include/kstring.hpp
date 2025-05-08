#pragma once

#include "kstring.h"
#include <string>
#include <vector>

namespace kstring
{
    std::vector<std::string> &tokenize( const char *str, char delim );
}
