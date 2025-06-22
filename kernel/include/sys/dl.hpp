#pragma once

namespace knl
{
    void *dl_open( const char* );
    void *dl_close( void* );
    void *sym_load( void *dl, const char* );
}



