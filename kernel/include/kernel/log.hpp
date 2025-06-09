#pragma once
#include <stdarg.h>
#include <mutex>

#define SYSLOG_QEMU


#ifdef SYSLOG_QEMU
    class syslog
    {
    private:
        inline static std::mutex m_mutex;
        // inline static knl::DummyLock m_mutex;
        inline static bool enabled;
        static void _putIndent();
        static void _print( const char *fmt, ... );

    public:
        static bool isEnabled() { return enabled; };
        static void enable() { enabled = true; };
        static void disable() { enabled = false; };
        static void pushIndent( int n=4 );
        static void popIndent( int n=4 );
        static int  getIndent();
        static void putIndent();
        static void vprint( const char *fmt, va_list vlist );
        static void print( const char *fmt, ... );
        static void println( const char *fmt, ... );

        template <typename... Args>
        syslog( const char *fmt, Args... args )
        {
            if (!enabled)
                return;
            std::lock_guard lock(m_mutex);
            syslog::_putIndent();
            syslog::_print("[");
            syslog::_print(fmt, args...);
            syslog::_print("]\n");
            syslog::_putIndent();
            syslog::_print("{\n");
            pushIndent();
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

#else
    class syslog
    {
    public:
        static bool isEnabled() { return false; };
        static void enable() {  };
        static void disable() {  };
        static void pushIndent( int ) {  };
        static void popIndent( int ) {  };
        static int  getIndent() { return 0; };
        static void putIndent() {  };
        static void print( const char*, ... ) {  };
        static void println( const char*, ... ) {  };

        template <typename... Args>
        syslog( const char*, Args... ) {  };
        void operator()( const char*, ... ) {  };
    };
#endif


