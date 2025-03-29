#pragma once

class syslog
{
public:
    static void enable();
    static void disable();
    static void kprintf( const char *fmt, ... );

    syslog( const char* );
    ~syslog();
    void operator()( const char *fmt, ... );

    // template <typename... Args>
    // void log( Args... args )
};

