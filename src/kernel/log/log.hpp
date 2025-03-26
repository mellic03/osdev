#pragma once


class syslog
{
public:
    syslog( const char* );
    ~syslog();
    void operator()( const char *fmt, ... );

    // template <typename... Args>
    // void log( Args... args )
};

