#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <cstring>
#include <cstdint>


enum TrigFun_: uint64_t
{
    TrigFun_SIN, TrigFun_ASIN,
    TrigFun_TAN, TrigFun_ATAN,
};

using fntype = double (*)(double);
static constexpr fntype lookup[4] = {
    std::sin, std::asin,
    std::tan, std::atan
};


template <typename dtype, int steps>
struct TrigTable
{
    uint64_t type;
    uint64_t size;
    uint64_t stride = sizeof(dtype);
    uint64_t count  = steps;
    uint64_t offset;
    dtype    m_lo;
    dtype    m_hi;
    dtype    m_data[steps];

    TrigTable( TrigFun_ tp, dtype lo, dtype hi )
    :   type(tp),
        size(sizeof(TrigTable<dtype, steps>)),
        offset(offsetof(TrigTable, m_data)),
        m_lo(lo),
        m_hi(hi)
    {
        fntype trigfn = lookup[tp];
        dtype  step   = (hi - lo) / dtype(steps);
        dtype  value  = lo;

        for (int i=0; i<steps; i++)
        {
            m_data[i] = trigfn(value);
            // std::cout << "offset: " << offset << "\n";
            // std::cout << "sin(" << value << ") == " << m_data[i] << "\n";
            value += step;
        }
    }

    void write( std::ofstream &stream )
    {
        stream.write(reinterpret_cast<char*>(this), sizeof(*this));
    }
};


int main( int argc, char **argv )
{
    // "../output/math_tables.bin"
    if (argc != 2)
    {
        std::cout << "Must supply output file path!\n";
        return 1;
    }

    std::ofstream stream(argv[1], std::ios::binary);

    TrigTable<float, 64>(TrigFun_SIN,   0.0, M_PI).write(stream);
    TrigTable<float, 64>(TrigFun_ASIN, -1.0, +1.0).write(stream);
    TrigTable<float, 64>(TrigFun_TAN,   0.0, M_PI).write(stream);
    TrigTable<float, 64>(TrigFun_ATAN, -1.0, +1.0).write(stream);

    return 0;
}

