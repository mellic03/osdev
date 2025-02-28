#pragma once
#include <stddef.h>
#include <stdint.h>


class ckBuffer
{
private:
    uint16_t *m_data;

public:
    int W, H, SZ;

    ckBuffer( int w, int h, uint16_t *buffer )
    :   m_data(buffer), W(w), H(h), SZ(w*h)
    {

    }

    void fill( int start, int end, uint16_t value )
    {
        for (int i=start; i<end; i++)
        {
            m_data[i] = value;
        }
    }


    void write( int idx, uint16_t value )
    {
        m_data[idx] = value;
    }

    void write( int row, int col, uint16_t value )
    {
        m_data[row*W + col] = value;
    }


    uint16_t *data()
    {
        return m_data;
    }

    uint16_t *operator[] ( int i )
    {
        return &(m_data[W*i]);
    }
};