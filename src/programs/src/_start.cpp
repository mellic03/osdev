#ifdef __is_kernel
    #undef __is_kernel
#endif

#include <libc.h>

// extern "C"
// {
//     using constructor_t = void (*)();
//     extern constructor_t __init_array_start[];
//     extern constructor_t __init_array_end[];

//     void ctor_init( void )
//     {
//         for (constructor_t *ctor = __init_array_start; ctor < __init_array_end-1; ctor++)
//         {
//             (*ctor)();
//         }
//     }

// }

extern int main( int argc, int64_t *argv );

extern "C"
int _start( int argc, int64_t *argv )
{
    // ctor_init();
    libc_init();

    int result = main(argc, argv);
    return result;
}



