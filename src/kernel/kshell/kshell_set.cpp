#include "kshell.hpp"
#include "../ksystem.hpp"
#include "../sde/sde.hpp"
#include <kernel/vfs.hpp>
#include <kstring.h>
#include <stdlib.h>

using namespace KShell;

struct VPair
{
    char *name;
    char *value;
};

static std::vector<VPair> vars;

int find_pair( const char *name )
{
    for (int i=0; i<vars.size(); i++)
    {
        if (strcmp(name, vars[i].name) == 0)
        {
            return i;
        }
    }

    return -1;
}



char *kshell_set( char *dst, int argc, char **argv )
{
    auto &cwd = kshell_tty->getCWD();

    if (argc == 1)
    {
        dst = kssprintf(dst, "set: arguments required");
        return dst;
    }

    for (int i=1; i<argc; i++)
    {
        if (count_ch(argv[i], ',') != 1)
        {
            return kssprintf(dst, "set: parse error at argument %d", i);
        }

        const char *A = argv[i];
        const char *B = seek_ch(A, ',');
        char *name = new char[B-A];
        strncpy(name, A, B-A);

        A = B+1;
        B = seek_brk(A, " \n\0");
        char *value = new char[B-A];
        strncpy(value, A, B-A);

        int idx = find_pair(name);

        if (idx == -1)
            vars.push_back({name, value});
        else
            vars[idx].value = value;

        dst = kssprintf(dst, "set: \"%s\",\"%s\"", name, value);

        if (strcmp(name, "sde::blend-mode") == 0)
        {
            int mode = atoi(value);
            sde::blendMode(mode);
        }
    }

    return dst;
}

