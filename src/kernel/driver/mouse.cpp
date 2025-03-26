#include "mouse.hpp"
#include "serial.hpp"
#include "pic.hpp"

using namespace idk;


uint8_t MousePointer[] = {
    0b11111111, 0b11100000, 
    0b11111111, 0b10000000, 
    0b11111110, 0b00000000, 
    0b11111100, 0b00000000, 
    0b11111000, 0b00000000, 
    0b11110000, 0b00000000, 
    0b11100000, 0b00000000, 
    0b11000000, 0b00000000, 
    0b11000000, 0b00000000, 
    0b10000000, 0b00000000, 
    0b10000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 
};

void MouseWait(){
    uint64_t timeout = 100000;
    while (timeout--){
        if ((IO::inb(0x64) & 0b10) == 0){
            return;
        }
    }
}

void MouseWaitInput(){
    uint64_t timeout = 100000;
    while (timeout--){
        if (IO::inb(0x64) & 0b1){
            return;
        }
    }
}

void MouseWrite(uint8_t value){
    MouseWait();
    IO::outb(0x64, 0xD4);
    MouseWait();
    IO::outb(0x60, value);
}

uint8_t MouseRead(){
    MouseWaitInput();
    return IO::inb(0x60);
}

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady = false;
int mouse_x;
int mouse_y;
int prev_mouse_x;
int prev_mouse_y;
// Point MousePosition;
// Point MousePositionOld;

void mouse_irq( kstackframe* )
{
    uint8_t data = IO::inb(0x60);

    switch(MouseCycle){
        case 0:
            if (MousePacketReady) break;
            if (data & 0b00001000 == 0) break;
            MousePacket[0] = data;
            MouseCycle++;
            break;
        case 1:
            if (MousePacketReady) break;
            MousePacket[1] = data;
            MouseCycle++;
            break;
        case 2:
            if (MousePacketReady) break;
            MousePacket[2] = data;
            MousePacketReady = true;
            MouseCycle = 0;
            break;
    }

    PIC::sendEOI(12);
}

void ProcessMousePacket(){
    if (!MousePacketReady) return;

        bool xNegative, yNegative, xOverflow, yOverflow;

        if (MousePacket[0] & PS2XSign){
            xNegative = true;
        }else xNegative = false;

        if (MousePacket[0] & PS2YSign){
            yNegative = true;
        }else yNegative = false;

        if (MousePacket[0] & PS2XOverflow){
            xOverflow = true;
        }else xOverflow = false;

        if (MousePacket[0] & PS2YOverflow){
            yOverflow = true;
        }else yOverflow = false;

        if (!xNegative){
            mouse_x += MousePacket[1];
            if (xOverflow){
                mouse_x += 255;
            }
        } else
        {
            MousePacket[1] = 256 - MousePacket[1];
            mouse_x -= MousePacket[1];
            if (xOverflow){
                mouse_x -= 255;
            }
        }

        if (!yNegative){
            mouse_y -= MousePacket[2];
            if (yOverflow){
                mouse_y -= 255;
            }
        } else
        {
            MousePacket[2] = 256 - MousePacket[2];
            mouse_y += MousePacket[2];
            if (yOverflow){
                mouse_y += 255;
            }
        }

        // if (mouse_x < 0) mouse_x = 0;
        // if (mouse_x > GlobalRenderer->TargetFramebuffer->Width-1) mouse_x = GlobalRenderer->TargetFramebuffer->Width-1;
        
        // if (mouse_y < 0) mouse_y = 0;
        // if (mouse_y > GlobalRenderer->TargetFramebuffer->Height-1) mouse_y = GlobalRenderer->TargetFramebuffer->Height-1;
        
        // GlobalRenderer->ClearMouseCursor(MousePointer, MousePositionOld);
        // GlobalRenderer->DrawOverlayMouseCursor(MousePointer, MousePosition, 0xffffffff);

        // if (MousePacket[0] & PS2Leftbutton){
        //     GlobalRenderer->PutChar('a', mouse_x, mouse_y);
        // }
        // if (MousePacket[0] & PS2Middlebutton){
            
        // }
        // if (MousePacket[0] & PS2Rightbutton){
        //     uint32_t colour = GlobalRenderer->Colour;
        //     GlobalRenderer->Colour = 0x0000ff00;
        //     GlobalRenderer->PutChar('a', mouse_x, mouse_y);
        //     GlobalRenderer->Colour = colour;
        // }

        MousePacketReady = false;
        // MousePositionOld = MousePosition;
}

void mouse_init()
{
 
    IO::outb(0x64, 0xA8); //enabling the auxiliary device - mouse

    MouseWait();
    IO::outb(0x64, 0x20); //tells the keyboard controller that we want to send a command to the mouse
    MouseWaitInput();
    uint8_t status = IO::inb(0x60);
    status |= 0b10;
    MouseWait();
    IO::outb(0x64, 0x60);
    MouseWait();
    IO::outb(0x60, status); // setting the correct bit is the "compaq" status byte

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
}

