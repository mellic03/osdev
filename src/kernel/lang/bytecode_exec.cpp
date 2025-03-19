// #include "./lang.hpp"
// #include <kdriver/serial.hpp>

// #include <string.h>
// #include <stdlib.h>
// #include <ctype.h>

// using namespace Lang;





// kret_t _bytecode_exec( const Exec *exec )
// {
//     uint32_t *base = (uint32_t*)exec;
//     uint32_t *end  = base + exec->end;
//     uint32_t *IP   = base + exec->text;
//     uint32_t  S[32];
//     uint32_t *SP = &(S[0]);

//     while (IP < end)
//     {
//         uint32_t cmd = IP[0];
//         uint32_t dwd = IP[1];
//         uint16_t op1 = (dwd >> 16);
//         uint16_t op2 = uint16_t(dwd);

//         switch (cmd)
//         {
//             default: SYSLOG("Failure at 0x%x", IP);   return KRET_FAILURE;
//             case 0x00:                                break;

//             case CMD_ADD:     base[op1] += op2;       break;
//             case CMD_ADD_REG: base[op1] += base[op2]; break;

//             case CMD_SUB:     base[op1] -= op2;       break;
//             case CMD_SUB_REG: base[op1] -= base[op2]; break;

//             case CMD_MUL:     base[op1] *= op2;       break;
//             case CMD_MUL_REG: base[op1] *= base[op2]; break;

//             case CMD_DIV:     base[op1] /= op2;       break;
//             case CMD_DIV_REG: base[op1] /= base[op2]; break;

//             case CMD_MOV:     base[op1] = op2;        break;
//             case CMD_MOV_REG: base[op1] = base[op2];  break;

//             case CMD_PSH:     *(SP++)   = dwd;        break;
//             case CMD_PSH_REG: *(SP++)   = base[dwd];  break;

//             case CMD_POP:  base[dwd] = *(SP--);       break;

//             case CMD_JMP:  IP = base + dwd;             break;
//             case CMD_EXT:  IP = end;                    break;

//             case CMD_OUT:     SYSLOG("%u", dwd);        break;
//             case CMD_OUT_REG: SYSLOG("%u", base[dwd]);  break;
//         }

//         IP+=2;
//     }

//     return KRET_SUCCESS;
// }



// int Lang::bytecode_exec( const Exec *exec )
// {
//     SYSLOG_BEGIN("bytecode_exec");

//     // uint32_t *base = new uint32_t[exec->size];
//     // Exec     *temp = (Exec*)base;
    
//     // *temp = {
//     //     .magic = exec->magic,
//     //     .size  = exec->size,
//     //     .data  = exec->text,
//     //     .text  = exec->text,
//     // };

//     kret_t ret = _bytecode_exec(exec);

//     SYSLOG_END();
//     return (ret) ? 0 : 1;
// }




// void Lang::bytecode_print( const Exec *exec )
// {
//     uint32_t *base = (uint32_t*)exec;
//     uint32_t *end  = base + exec->end;
//     uint32_t *IP   = base;

//     while (IP < end)
//     {
//         union {
//             uint32_t udata;
//             uint32_t fdata;
//         };

//         uint32_t cmd = IP[0];
//         uint32_t dwd = IP[1];

//         // uint16_t op1  = (udata >> 16);
//         // uint16_t op2  = uint16_t(udata);

//         SYSLOG("0x%x: 0x%x 0x%x", IP-base, cmd, dwd);
//         // SYSLOG("0x%x: 0x%x 0x%x 0x%x", IP, cmd, op1, op2);

//         IP+=2;
//     }
// }



