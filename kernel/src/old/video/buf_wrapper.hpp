#pragma once

template <typename T>
struct buf_wrapper
{
    int W;
    T *buf;

    buf_wrapper()
    :   W(0), buf(0x00) {  };

    buf_wrapper( int w, T *b )
    :   W(w), buf(b) {  };

    inline constexpr
    T *operator[](int i)
    {
        return &(buf[W*i]);
    };

    
    inline constexpr
    const T *operator[](int i) const
    {
        return &(buf[W*i]);
    };

};

