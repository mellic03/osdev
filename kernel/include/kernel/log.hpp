#pragma once
#include <mutex>

class syslog
{
private:
    inline static std::mutex m_mutex;
    inline static bool enabled;
    static void _putIndent();
    static void _print( const char *fmt, ... );

public:
    static bool isEnabled() { return enabled; };
    static void enable() { enabled = true; };
    static void disable() { enabled = false; };
    static void pushIndent( int n=4 );
    static void popIndent( int n=4 );
    static void putIndent();
    static void print( const char *fmt, ... );
    static void println( const char *fmt, ... );

    template <typename... Args>
    syslog( const char *fmt, Args... args )
    {
        if (!enabled)
            return;
        // std::lock_guard lock(m_mutex);
        m_mutex.lock();

        syslog::_putIndent();
        syslog::_print("[");
        syslog::_print(fmt, args...);
        syslog::_print("]\n");
        syslog::_putIndent();
        syslog::_print("{\n");
        pushIndent();
    
        m_mutex.unlock();
    }

    ~syslog()
    {
        if (!enabled)
            return;
        std::lock_guard lock(m_mutex);
        popIndent();
        syslog::_putIndent();
        syslog::_print("}\n");
    }

    void operator()( const char *fmt, ... );

};

