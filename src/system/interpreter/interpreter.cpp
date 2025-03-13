#include "./interpreter.hpp"
#include <cctype.hpp>
#include <cstdio.hpp>
#include <cstring.hpp>






// #define REGISTER_UNION(__c__) \
// union { \
//     uint64_t r##__c__ ##x; \
//     uint32_t e##__c__ ##x; \
//     uint16_t \
//     ##__c__ ##x; \
// }; \


struct CPU_CTX
{
    uint8_t stack[128];

    uint32_t pc;
    uint32_t sp;

    // REGISTER_UNION(a)
    // REGISTER_UNION(b)
    // REGISTER_UNION(c)
    // REGISTER_UNION(d)

    union
    {
        uint64_t rax;
        uint32_t eax;
        uint16_t ax;
    };

};

#undef REGISTER_UNION


static CPU_CTX ctx;


const char*
asm_MOV( const char *src )
{
    // char tokens[3] = {nullptr, nullptr, nullptr};

}




int
asm_interpreter::Exec( const char *src )
{
    ctx.pc = 0;
    ctx.sp = 0;

    auto test = "mov rax, 15\n";
    src = test;

    while (*src)
    {
        if (std::strcmp(src, "mov") == 0)
        {
            const char *res = asm_MOV(src);
        
            if (res)
            {
                src = res;
            }
        }

        src++;
    }

    // return check_function_call(src);

    // while (*src)
    // {
    //     std::printf("%c ", *src);
    //     src++;
    // }
}

