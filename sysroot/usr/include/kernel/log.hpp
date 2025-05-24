#pragma once
#include <mutex>

class syslog
{
private:
    // void _print( const char *fmt, ... );
    inline static std::mutex m_mutex;
    // inline static knl::dummy_lock m_mutex;
    inline static bool enabled;
    static void _putIndent();


public:
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
        syslog::print("[");
        syslog::print(fmt, args...);
        syslog::print("]\n");
        syslog::_putIndent();
        syslog::print("{\n");
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
        syslog::print("}\n");
    }

    void operator()( const char *fmt, ... );

};

