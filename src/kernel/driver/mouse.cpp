#include "mouse.hpp"
#include "pic.hpp"
#include "../interrupt/interrupt.hpp"
#include <kernel/vfs.hpp>
#include <kernel/ioport.hpp>
#include <kernel/log.hpp>

#include <kscancode.h>
#include <kmalloc.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


// static void mouse_init();
// static void irq_handler( kstackframe* );
// static void driver_main( void* );


ivec2 hwdi_PS2Mouse::position   = ivec2(100);
bool  hwdi_PS2Mouse::left       = false;
bool  hwdi_PS2Mouse::right      = false;
bool  hwdi_PS2Mouse::prev_left  = false;
bool  hwdi_PS2Mouse::prev_right = false;

bool &currLeft  = hwdi_PS2Mouse::left;
bool &currRight = hwdi_PS2Mouse::right;
bool &prevLeft  = hwdi_PS2Mouse::prev_left;
bool &prevRight = hwdi_PS2Mouse::prev_right;


// hwdi_PS2Mouse::hwdi_PS2Mouse()
// :   hwDriverInterface("PS2 Mouse")
// {
//     this->irqno   = 12;
//     this->handler = kdriver::ps2_mouse::irq_handler;
//     this->entry   = kdriver::ps2_mouse::driver_main;
// }


// void
// hwdi_PS2Mouse::loadIrqHandler()
// {
//     mouse_init();
//     kernel::registerIRQ(this->irqno, this->handler);
//     PIC::unmask(this->irqno);
// }







using namespace idk;

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000100
#define PS2Rightbutton 0b00000010
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

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


void ProcessMousePacket()
{
    if (!MousePacketReady)
    {
        return;
    }

    bool xNegative, yNegative, xOverflow, yOverflow;

    prevLeft  = currLeft;
    prevRight = currRight;

    currLeft  = (MousePacket[0] & 0b01);
    currRight = (MousePacket[0] & 0b10);

    if (currLeft != prevLeft)
    {
        if (currLeft && hwdi_PS2Mouse::onLeftDown)
            hwdi_PS2Mouse::onLeftDown();
        else if (hwdi_PS2Mouse::onLeftUp)
            hwdi_PS2Mouse::onLeftUp();
    }

    if (currRight != prevRight)
    {
        if (currRight && hwdi_PS2Mouse::onRightDown)
            hwdi_PS2Mouse::onRightDown();
        else if (hwdi_PS2Mouse::onRightUp)
            hwdi_PS2Mouse::onRightUp();
    }

    xNegative = (MousePacket[0] & PS2XSign);
    yNegative = (MousePacket[0] & PS2YSign);
    xOverflow = (MousePacket[0] & PS2XOverflow);
    yOverflow = (MousePacket[0] & PS2YOverflow);

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

    hwdi_PS2Mouse::position = ivec2(mouse_x, mouse_y);
    MousePacketReady = false;
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



void kdriver::ps2_mouse::irq_handler( kstackframe* )
{
    uint8_t data = IO::inb(0x60);
    switch(MouseCycle){
        case 0:
            if (MousePacketReady) break;
            if ((data & 0b00001000) == 0) break;
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
}


void kdriver::ps2_mouse::driver_main( void* )
{
    while (true)
    {
        ProcessMousePacket();
        kthread::yield();
    }
}



