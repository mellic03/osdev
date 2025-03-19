// #pragma once

// #include <stddef.h>
// #include <stdint.h>


// namespace Lang
// {
//     static constexpr size_t ADDR_START    = 8;
//     static constexpr size_t ADDR_MAX      = 0xFF;
//     static constexpr size_t ADDR_RETVALUE = 62;

//     struct Exec
//     {
//         uint32_t magic; // 0xDEADBEBE
//         uint32_t end;
//         uint32_t data;
//         uint32_t text;
//         // uint32_t *addr;
//     } __attribute__ ((aligned(4)));


//     Exec *bytecode_assemble( const char *src );
//     int   bytecode_exec( const Exec* );
//     void  bytecode_print( const Exec* );
    
//     enum VM_BYTECODE: uint32_t
//     {
//         CMD_ADD     = 0xA0,
//         CMD_ADD_IMM = CMD_ADD,
//         CMD_ADD_REG = CMD_ADD + 1,

//         CMD_SUB     = 0xA2,
//         CMD_SUB_IMM = CMD_SUB,
//         CMD_SUB_REG = CMD_SUB + 1,

//         CMD_DIV     = 0xA4,
//         CMD_DIV_IMM = CMD_DIV,
//         CMD_DIV_REG = CMD_DIV + 1,

//         CMD_MUL     = 0xA6,
//         CMD_MUL_IMM = CMD_MUL,
//         CMD_MUL_REG = CMD_MUL + 1,

//         CMD_MOV     = 0xA8,
//         CMD_MOV_IMM = CMD_MOV,
//         CMD_MOV_REG = CMD_MOV + 1,

//         CMD_PSH     = 0xB0,
//         CMD_PSH_IMM = CMD_PSH,
//         CMD_PSH_REG = CMD_PSH | 1,

//         CMD_POP     = 0xB2,
//         CMD_POP_2 = CMD_POP,
//         CMD_POP_3 = CMD_POP,

//         CMD_JMP     = 0xB4,
//         CMD_JMP_IMM = CMD_JMP,
//         CMD_JMP_REG = CMD_JMP + 1,

//         CMD_OUT     = 0xB6,
//         CMD_OUT_IMM = CMD_OUT,
//         CMD_OUT_REG = CMD_OUT + 1,

//         CMD_EXT,
//     };

//     static constexpr const char *REG_RAX = "0x0004";
//     static constexpr const char *REG_EAX = "0x0005";

//     static constexpr const char *REG_RBX = "0x0006";
//     static constexpr const char *REG_EBX = "0x0008";

//     static constexpr const char *REG_RDI = "0x0007";
//     static constexpr const char *REG_EDI = "0x0008";


// }

