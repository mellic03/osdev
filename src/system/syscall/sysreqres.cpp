#include "./syscall.hpp"


idk::SysRequest::SysRequest( uint32_t type, const char *buffer )
{
    this->type = type;
    std::strcpy(&(buf[0]), buffer);
}


