#pragma once
#include <kmemxx.hpp>

namespace knl
{
    struct kTTY
    {
        static constexpr size_t BUF_W = 80; 
        static constexpr size_t BUF_H = 35; 
        char     m_buf[BUF_H*BUF_W];
        int      m_idx;
    
        kTTY()
        {
            m_idx = 0;
            kmemset((uint8_t*)m_buf, 0, sizeof(m_buf));
        };

        void putch( char );
        void putstr( const char *s ) { while (*s) putch(*(s++)); };
        void scroll();
        void scrolln( int n );
    };
    
}

