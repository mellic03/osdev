#include <cxxabi>
// #include <kpanic.hpp>

extern "C"
{


atexit_func_entry_t __atexit_funcs[ATEXIT_MAX_FUNCS];
uarch_t __atexit_func_count = 0;

void *__dso_handle = 0;

int __cxa_atexit(void (*func)(void *), void *objptr, void *dso)
{
    if (__atexit_func_count >= ATEXIT_MAX_FUNCS) return -1;

    __atexit_funcs[__atexit_func_count].destructor_func = func;
    __atexit_funcs[__atexit_func_count].obj_ptr = objptr;
    __atexit_funcs[__atexit_func_count].dso_handle = dso;
    __atexit_func_count++;

    return 0;
};



void __cxa_finalize(void *func)
{
    uarch_t i = __atexit_func_count;
    if (func == nullptr)
    {
        while (i--)
        {
            if (__atexit_funcs[i].destructor_func)
            {
                (*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
            }
        }
        return;
    }

    while (i--)
    {
        if (__atexit_funcs[i].destructor_func == (void(*)(void*))(func))
        {
            (*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
            __atexit_funcs[i].destructor_func = 0;
        };
    };
};



namespace __cxxabiv1
{
    int __cxa_guard_acquire( uint64_t *g )
    {
        return !__atomic_load_n(g, __ATOMIC_SEQ_CST)
            && !__atomic_exchange_n(g, 0, __ATOMIC_SEQ_CST);
    }

    void __cxa_guard_release( uint64_t *g )
    {
        __atomic_store_n(g, 1, __ATOMIC_SEQ_CST);
    }

    void __cxa_guard_abort( uint64_t* )
    {
        
    }
}


void __cxa_pure_virtual()
{
    // kpanic("[__cxa_pure_virtual] Pure virtual function called!");
}


}