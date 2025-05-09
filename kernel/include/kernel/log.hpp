#pragma once

class syslog
{
private:
    // void _print( const char *fmt, ... );
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
        if (!enabled)
            return;
        syslog::print("[");
        syslog::printf(fmt, args...);
        syslog::printf("]\n");
        syslog::print("{\n");
        pushIndent();
    }

    ~syslog()
    {
        if (!enabled)
            return;
        popIndent();
        syslog::print("}\n");
    }

    void operator()( const char *fmt, ... );

};

