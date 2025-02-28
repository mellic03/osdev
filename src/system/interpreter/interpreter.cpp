#include "./interpreter.hpp"
#include <stdc++/cctype.hpp>
#include <stdc++/cstdio.hpp>
#include <stdc++/cstring.hpp>


enum ck_dtype_t: uint32_t
{
    DTYPE_I8, DTYPE_I16, DTYPE_I32,
    DTYPE_U8, DTYPE_U16, DTYPE_U32,

    DTYPE_PTR = DTYPE_U32
};


enum ck_opcode_t: uint32_t
{
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,

    OP_ASSIGN,
    OP_RETURN,

    OP_CALL,
    OP_RET
};


enum ck_token_t: uint32_t
{
    TOKEN_TYPE_NAME,
    TOKEN_VARIABLE_NAME,

    TOKEN_OPERATOR,
    TOKEN_EXPRESSION,

    TOKEN_SCOPE_ENTER,
    TOKEN_SCOPE_EXIT,

    TOKEN_END
};


struct ckNode
{
    ck_token_t type;

    union
    {
        uint32_t opcode;
        uint32_t datatype;
    };


    ckNode *left, *right;
};



static struct {
    uint8_t stack[128];

    uint32_t pc;
    uint32_t sp;

    void push( uint8_t data )
    {
        stack[sp++] = data;
    }

    uint8_t pop()
    {
        return stack[sp--];
    }

    void call( uint32_t addr )
    {
        push(addr);
        pc = addr;
    }

    void ret()
    {
        pc = pop();
    }

} ctx;





int token_match( const char *src, const char *token )
{
    uint32_t len = std::strlen(token);

    for (uint32_t i=0; i<len; i++)
    {
        if (src[i] != token[i])
        {
            return 0;
        }
    }

    return len;
}



int name_match( const char *src, const char *name )
{
    uint32_t len = std::strlen(name);

    switch (src[len])
    {
        default: return 0;
        case '(':
        case ')':
        case ';': break;
    }

    for (uint32_t i=0; i<len; i++)
    {
        if (src[i] != name[i])
        {
            return 0;
        }
    }

    return len;
}



int check_typename( const char *src )
{
    char type_names[][16] = {
        "int8",  "int16", "int32",
        "uint8", "uint16", "uint32",
        "ptr_t"
    };

    for (char *name: type_names)
    {
        if (token_match(src, name))
        {
            return std::strlen(name);
        }
    }

    return 0;
}



/**
 * <typename> <label> = <expression> END
 * <typename> END
 */
int check_typename_label( const char *src )
{
    while (*src)
    {


        src++;
    }


    return 0;
}


void ckNode_op( ckNode &node )
{
    switch (node.opcode)
    {
        default: break;
        case OP_RETURN: break;
    }
}

int check_function_name( const char *src )
{
    char fn_names[][16] = {
        "myFunction",
        "doThing",
        "addXY"
    };


    int offset = 0;

    for (auto name: fn_names)
    {
        offset = token_match(src, name);

        if (offset)
        {
            return std::strlen(name);
        }
    }

    return 0;
}



/*
    Function call:
    <function_name> ( <ARGS>... ) END
*/

int check_function_call( const char *src )
{
    char buf[32];

    int offset;
    
    if (!(offset = check_function_name(src)))
    {
        return 0;
    }

    std::memcpy(buf, src, offset);
    buf[offset] = '\0';


    // src += offset;

    // if (*src != '(')
    // {
    //     return 0;
    // }


    std::printf("[ckInterpreter] function call: %s\n", buf);
    // function name == [src[0], src[end]]

    return 0;
}


void ckNode_parse( ckNode &node )
{
    switch (node.type)
    {
        default: break;
        case TOKEN_OPERATOR: ckNode_op(node); break;
    }
}



int
ckInterpreter::Exec( const char *src )
{
    ctx.pc = 0;
    ctx.sp = 0;

    return check_function_call(src);

    // while (*src)
    // {
    //     std::printf("%c ", *src);
    //     src++;
    // }
}

