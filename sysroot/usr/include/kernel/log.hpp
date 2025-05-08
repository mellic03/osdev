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
    static void kprintf( const char *fmt, ... );
    static void print( const char *fmt, ... );

    template <typename... Args>
    syslog( const char *fmt, Args... args )
    {
        if (!enabled)
            return;
        print("[");
        kprintf(fmt, args...);
        kprintf("]\n");
        print("{\n");
        pushIndent();
    }

    ~syslog()
    {
        if (!enabled)
            return;
        popIndent();
        print("}\n");
    }

    void operator()( const char *fmt, ... );

};

