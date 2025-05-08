#pragma once
#include <inplace_vector.hpp>


namespace idk
{
    template <typename T, size_t cap>
    class stack_vector: public idk::inplace_vector<T>
    {
    private:
        T m_data[cap];


    public:
        stack_vector()
        :   inplace_vector<T>(m_data, cap)
        {

        }

        stack_vector( const T &value )
        :   stack_vector()
        {
            for (size_t i=0; i<cap; i++)
            {
                m_data[i] = value;
            }
        }
        

    };

}
