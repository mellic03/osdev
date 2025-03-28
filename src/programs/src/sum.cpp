#include <stdint.h>


int64_t main( int argc, int64_t *argv )
{
    if (argc == 2)
    {
        return argv[0] + argv[1];
    }

    return 0;
}

