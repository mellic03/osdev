// #include "./lang.hpp"
// #include <kmemory/memory.hpp>
// #include <kdriver/serial.hpp>
// #include <kinplace/inplace_stack.hpp>

// #include <string.h>
// #include <stdlib.h>
// #include <ctype.h>

// using namespace Lang;

// // static uint8_t __table[0xFF];

// // uint32_t get_code( const char *tok )
// // {
// //     uint32_t code = (uint32_t(tok[0]) << 24)
// //                   + (uint32_t(tok[1]) << 16)
// //                   + (uint32_t(tok[2]) <<  8)
// //                   + (uint32_t(tok[3]) <<  0);

// //     return code;
// // }


// static void replace_newlines( char *src )
// {
//     char *ptr = strchr(src, '\n');

//     while (ptr)
//     {
//         *ptr = ' ';
//         ptr = strchr(ptr+1, '\n');
//     }

// }


// // mov   rax 15
// // mov        

// static char *reduce_whitespace( char *src )
// {
//     char *base = (char*)kmalloc(strlen(src)+1);
//     char *dst  = base;

//     while (src)
//     {
//         if (*src == ' ' && *(src+1) == ' ')
//         {
//             // *(dst++) = *src;
//         }

//         else
//         {
//             *dst = *src;
//             dst++;
//         }

//         src++;
//     }
    
//     *dst = '\0';
//     strcpy(base, src);

//     kfree(src);
//     return base;
// }




// static char *CURR_TOKEN;

// kret_t get_command( uint32_t *cmd );
// kret_t interpret_operands( uint32_t *cmd, uint32_t *data );



// Lang::Exec *Lang::bytecode_assemble( const char *insrc )
// {
//     SYSLOG_BEGIN("bytecode_compile");

//     char *srcbuf = (char*)kmalloc(strlen(insrc)+1);
//     strcpy(srcbuf, insrc);
//     replace_newlines(srcbuf);
//     srcbuf = reduce_whitespace(srcbuf);
//     SYSLOG("source: \"%s\"", srcbuf);
    
//     CURR_TOKEN = strtok(srcbuf, " ");


//     uint32_t *base = (uint32_t*)kmalloc(ADDR_MAX*sizeof(uint32_t));
//     memset(base, 0, ADDR_MAX*sizeof(uint32_t));

//     Exec *exec = (Exec*)base;

//     *exec = {
//         .magic = 0xDEADBEBE,
//         .end   = ADDR_MAX,
//         .data  = 8,
//         .text  = 16
//     };


//     uint32_t *IP = base + exec->text;

//     while (CURR_TOKEN)
//     {
//         SYSLOG_BEGIN("compile loop");
//         SYSLOG("CURR_TOKEN: \"%s\"", CURR_TOKEN);

//         uint32_t cmd  = 0;
//         uint32_t data = 0;

//         if (!get_command(&cmd))
//         {
//             kfree(base);
//             return nullptr;
//         }

//         if (!interpret_operands(&cmd, &data))
//         {
//             kfree(base);
//             return nullptr;
//         }

//         SYSLOG("bytecode: 0x%x 0x%x", cmd, data);

//         IP[0] = cmd;
//         IP[1] = data;
//         IP += 2;

//         CURR_TOKEN = strtok(nullptr, " ");
//         SYSLOG_END();
//     }

//     exec->end = IP - base;

//     SYSLOG_RET(exec);

// }













// static char *replace_regnames( uint32_t *cmd, char *token )
// {
//     static const char *names[6] = {
//         "rax", "eax", "rbx", "rbx", "rdi", "edi"
//     };

//     static const char *addresses[6] = {
//         REG_RAX, REG_EAX, REG_RBX, REG_EBX, REG_RDI, REG_EDI
//     };

//     static char buf[8];


//     for (int i=0; i<6; i++)
//     {
//         if (strcmp(token, names[i]) == 0)
//         {
//             strcpy(buf, addresses[i]);
//             if (cmd) *cmd |= 0x01;
//             return buf;
//         }
//     }

//     return token;
// }






// kret_t get_command( uint32_t *cmd )
// {
//     SYSLOG_BEGIN("get_command");

//     if (strncmp(CURR_TOKEN, "add", 3)==0) *cmd=CMD_ADD;
//     if (strncmp(CURR_TOKEN, "sub", 3)==0) *cmd=CMD_SUB;
//     if (strncmp(CURR_TOKEN, "mul", 3)==0) *cmd=CMD_MUL;
//     if (strncmp(CURR_TOKEN, "div", 3)==0) *cmd=CMD_DIV;
//     if (strncmp(CURR_TOKEN, "mov", 3)==0) *cmd=CMD_MOV;
//     if (strncmp(CURR_TOKEN, "psh", 3)==0) *cmd=CMD_PSH;
//     if (strncmp(CURR_TOKEN, "pop", 3)==0) *cmd=CMD_POP;
//     if (strncmp(CURR_TOKEN, "jmp", 3)==0) *cmd=CMD_JMP;
//     if (strncmp(CURR_TOKEN, "out", 3)==0) *cmd=CMD_OUT;
//     if (strncmp(CURR_TOKEN, "ext", 3)==0) *cmd=CMD_EXT;

//     if (*cmd)
//     {
//         SYSLOG("cmd:  0x%x", *cmd);
//         SYSLOG_RET(KRET_SUCCESS);
//     }

//     SYSLOG("cmd:  0x00");
//     SYSLOG_RET(KRET_FAILURE);
// }




// kret_t interpret_operands( uint32_t *cmd, uint32_t *data )
// {
//     switch (*cmd)
//     {
//         default:
//             return KRET_FAILURE;

//         case CMD_EXT:
//             *data=0x0000;
//             return KRET_SUCCESS;

//         case CMD_PSH:
//         case CMD_POP:
//         case CMD_JMP:
//         case CMD_OUT:    goto cmd_dword;

//         case CMD_ADD:
//         case CMD_SUB:
//         case CMD_MUL:
//         case CMD_DIV:
//         case CMD_MOV:    goto cmd_word_word;

//     }


// cmd_dword:
//     {
//         CURR_TOKEN = strtok(nullptr, " ");
//         char *op1str = CURR_TOKEN;
//         op1str = replace_regnames(cmd, op1str);
//         SYSLOG("op1 str:  \"%s\"", op1str);

//         *data = atoi(op1str);
//         return KRET_SUCCESS;
//     }


// cmd_word_word:
//     {
//         CURR_TOKEN = strtok(nullptr, " ");
//         char *op1str = CURR_TOKEN;
//         op1str = replace_regnames(nullptr, op1str);
//         SYSLOG("op1 str:  \"%s\"", op1str);
//         uint16_t op1 = atoi(op1str);

//         CURR_TOKEN = strtok(nullptr, " ");
//         char *op2str = CURR_TOKEN;
//         op2str = replace_regnames(cmd, op2str);
//         SYSLOG("op2 str:  \"%s\"", op2str);
//         uint16_t op2 = atoi(op2str);

//         *data = (uint32_t(op1) << 16) + op2;

//         return KRET_SUCCESS;
//     }

// }




