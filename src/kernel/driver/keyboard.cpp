#include "keyboard.hpp"
#include "serial.hpp"
#include "pic.hpp"
#include "../kfs/kfs.hpp"

#include <kscancode.h>
#include <kmalloc.h>
#include <stdio.h>
#include <string.h>

using namespace idk;


void keyboard_irq_handler( kstackframe *frame )
{
    uint8_t code = IO::inb(0x60);

    KFile_write(KFS::kdevscn, &code, 1);
    // printf("[keyboard] code=%u\n", code);

    PIC::sendEOI(1);
}


char scode_getalpha( uint8_t code )
{
    switch (code)
    {
        default: return '\0';
        case DOWN_Q: return 'Q';
        case DOWN_W: return 'W';
        case DOWN_E: return 'E';
        case DOWN_R: return 'R';
        case DOWN_T: return 'T';
        case DOWN_Y: return 'Y';
        case DOWN_U: return 'U';
        case DOWN_I: return 'I';
        case DOWN_O: return 'O';
        case DOWN_P: return 'P';
        case DOWN_A: return 'A';
        case DOWN_S: return 'S';
        case DOWN_D: return 'D';
        case DOWN_F: return 'F';
        case DOWN_G: return 'G';
        case DOWN_H: return 'H';
        case DOWN_J: return 'J';
        case DOWN_K: return 'K';
        case DOWN_L: return 'L';
        case DOWN_Z: return 'Z';
        case DOWN_X: return 'X';
        case DOWN_C: return 'C';
        case DOWN_V: return 'V';
        case DOWN_B: return 'B';
        case DOWN_N: return 'N';
        case DOWN_M: return 'M';
    }
}

