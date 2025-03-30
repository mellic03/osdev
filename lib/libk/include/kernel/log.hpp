#pragma once

class syslog
{
private:
    // void _print( const char *fmt, ... );

public:
    static void enable();
    static void disable();
    static void pushIndent( int n=4 );
    static void popIndent( int n=4 );
    static void kprintf( const char *fmt, ... );

    template <typename... Args>
    syslog( const char *fmt, Args... args )
    {
        print("[");
        kprintf(fmt, args...);
        kprintf("]\n");
        print("{\n");
        pushIndent();
    }

    ~syslog()
    {
        popIndent();
        print("}\n");
    }

    void print( const char *fmt, ... );
    void operator()( const char *fmt, ... );

};

