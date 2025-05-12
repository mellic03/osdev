#pragma once
#include <mutex>

class syslog
{
private:
    // void _print( const char *fmt, ... );
    inline static std::mutex m_mutex;
    static bool enabled;

public:
    static void enable();
    static void disable();
    static void pushIndent( int n=4 );
    static void popIndent( int n=4 );
    static void printf( const char *fmt, ... );
    static void println( const char *fmt, ... );
    static void print( const char *fmt, ... );

    template <typename... Args>
    syslog( const char *fmt, Args... args )
    {
        std::lock_guard lock(m_mutex);
        syslog::print("[");
        syslog::printf(fmt, args...);
        syslog::printf("]\n");
        syslog::print("{\n");
        pushIndent();
    }

    ~syslog()
    {
        std::lock_guard lock(m_mutex);
        popIndent();
        syslog::print("}\n");
    }

    void operator()( const char *fmt, ... );

};

