#include "./syscall.hpp"


idk::SysRequest::SysRequest( uint32_t type, uint32_t flags, const char *buffer )
{
    this->type  = type;
    this->flags = flags;
    std::strcpy(&(buf[0]), buffer);
}


