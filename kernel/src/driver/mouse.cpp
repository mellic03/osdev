// #include <driver/mouse.hpp>
// #include <driver/pic.hpp>
// // #include "pic.hpp"
// // #include <kernel/interrupt.hpp>

// #include <kthread.hpp>
// #include <kernel/log.hpp>
// #include <arch/io.hpp>
// #include <kernel/interrupt.hpp>
// #include <kernel/kscancode.h>

// #include <kmalloc.h>
// #include <stdio.h>
// #include <string.h>
// #include <ctype.h>


// static void mouse_init();
// static void irq_handler( intframe_t* );
// static void driver_main( void* );

// static void MouseWrite(uint8_t);
// static uint8_t MouseRead();


// ivec2 hwPS2Mouse::position   = ivec2(100);
// bool  hwPS2Mouse::left       = false;
// bool  hwPS2Mouse::right      = false;
// bool  hwPS2Mouse::prev_left  = false;
// bool  hwPS2Mouse::prev_right = false;

// bool &currLeft  = hwPS2Mouse::left;
// bool &currRight = hwPS2Mouse::right;
// bool &prevLeft  = hwPS2Mouse::prev_left;
// bool &prevRight = hwPS2Mouse::prev_right;




// #define PS2Leftbutton 0b00000001
// #define PS2Middlebutton 0b00000100
// #define PS2Rightbutton 0b00000010
// #define PS2XSign 0b00010000
// #define PS2YSign 0b00100000
// #define PS2XOverflow 0b01000000
// #define PS2YOverflow 0b10000000

// void MouseWait()
// {
//     uint64_t timeout = 100000;
//     while (timeout--){
//         if ((IO::inb(0x64) & 0b10) == 0){
//             return;
//         }
//     }
// }

// void MouseWaitInput()
// {
//     uint64_t timeout = 100000;
//     while (timeout--){
//         if (IO::inb(0x64) & 0b1){
//             return;
//         }
//     }
// }

// static void MouseWrite( uint8_t value )
// {
//     MouseWait();
//     IO::outb(0x64, 0xD4);
//     MouseWait();
//     IO::outb(0x60, value);
// }

// uint8_t MouseRead(){
//     MouseWaitInput();
//     return IO::inb(0x60);
// }

// uint8_t MouseCycle = 0;
// uint8_t MousePacket[4];
// bool MousePacketReady = false;
// int mouse_x;
// int mouse_y;
// int prev_mouse_x;
// int prev_mouse_y;
// // Point MousePosition;
// // Point MousePositionOld;


// void ProcessMousePacket()
// {
//     if (!MousePacketReady)
//     {
//         return;
//     }

//     bool xNegative, yNegative, xOverflow, yOverflow;

//     prevLeft  = currLeft;
//     prevRight = currRight;

//     currLeft  = (MousePacket[0] & 0b01);
//     currRight = (MousePacket[0] & 0b10);

//     if (currLeft != prevLeft)
//     {
//         if (currLeft && hwPS2Mouse::onLeftDown)
//             hwPS2Mouse::onLeftDown();
//         else if (hwPS2Mouse::onLeftUp)
//             hwPS2Mouse::onLeftUp();
//     }

//     if (currRight != prevRight)
//     {
//         if (currRight && hwPS2Mouse::onRightDown)
//             hwPS2Mouse::onRightDown();
//         else if (hwPS2Mouse::onRightUp)
//             hwPS2Mouse::onRightUp();
//     }

//     xNegative = (MousePacket[0] & PS2XSign);
//     yNegative = (MousePacket[0] & PS2YSign);
//     xOverflow = (MousePacket[0] & PS2XOverflow);
//     yOverflow = (MousePacket[0] & PS2YOverflow);

//     if (!xNegative){
//         mouse_x += MousePacket[1];
//         if (xOverflow){
//             mouse_x += 255;
//         }
//     } else
//     {
//         MousePacket[1] = 256 - MousePacket[1];
//         mouse_x -= MousePacket[1];
//         if (xOverflow){
//             mouse_x -= 255;
//         }
//     }

//     if (!yNegative){
//         mouse_y -= MousePacket[2];
//         if (yOverflow){
//             mouse_y -= 255;
//         }
//     } else
//     {
//         MousePacket[2] = 256 - MousePacket[2];
//         mouse_y += MousePacket[2];
//         if (yOverflow){
//             mouse_y += 255;
//         }
//     }

//     hwPS2Mouse::position = ivec2(mouse_x, mouse_y);
//     MousePacketReady = false;
// }



// static void mouse_init()
// {
//     IO::outb(0x64, 0xA8); // enabling the auxiliary device - mouse

//     MouseWait();
//     IO::outb(0x64, 0x20); // tells the keyboard controller that we want to send a command to the mouse
//     MouseWaitInput();
//     uint8_t status = IO::inb(0x60);
//     status |= 0b10;
//     MouseWait();
//     IO::outb(0x64, 0x60);
//     MouseWait();
//     IO::outb(0x60, status); // setting the correct bit is the "compaq" status byte

//     MouseWrite(0xF6);
//     MouseRead();

//     MouseWrite(0xF4);
//     MouseRead();

// }



// // void kdriver::ps2_mouse::irq_handler( kstackframe* )
// static void irq_handler( intframe_t* )
// {
//     uint8_t data = IO::inb(0x60);
//     switch(MouseCycle){
//         case 0:
//             if (MousePacketReady) break;
//             if ((data & 0b00001000) == 0) break;
//             MousePacket[0] = data;
//             MouseCycle++;
//             break;
//         case 1:
//             if (MousePacketReady) break;
//             MousePacket[1] = data;
//             MouseCycle++;
//             break;
//         case 2:
//             if (MousePacketReady) break;
//             MousePacket[2] = data;
//             MousePacketReady = true;
//             MouseCycle = 0;
//             break;
//     }

//     // PIC::sendEOI(12);
// }


// // void kdriver::ps2_mouse::driver_main( void* )
// static void driver_main( void* )
// {
//     while (true)
//     {
//         ProcessMousePacket();
//         kthread::yield();
//     }
// }






// hwPS2Mouse::hwPS2Mouse()
// :   hwDriverInterface("PS2 Mouse")
// {
//     this->irqno   = 12;
//     this->handler = irq_handler;
//     this->entry   = driver_main;
// }

// void hwPS2Mouse::loadIrqHandler()
// {
//     mouse_init();
//     // setSampleRate(50);
//     // setResolution(0x02);
//     // setScalingLinear();
//     CPU::installIRQ(this->irqno, this->handler);
//     PIC::unmask(this->irqno);
// }

// void hwPS2Mouse::setSampleRate( uint8_t value )
// {
//     MouseWrite(0xF3);  MouseRead();
//     MouseWrite(value); MouseRead();
// }

// void hwPS2Mouse::setResolution( uint8_t value )
// {
//     MouseWrite(0xE8);  MouseRead();
//     MouseWrite(value); MouseRead();
// }

// void hwPS2Mouse::setScalingNonLinear()
// {
//     MouseWrite(0xE7); MouseRead();
// }

// void hwPS2Mouse::setScalingLinear()
// {
//     MouseWrite(0xE6); MouseRead();
// }

