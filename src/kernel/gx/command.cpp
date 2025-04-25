#include "command.hpp"
#include <kpanic.h>

extern void gx_ExecCommand_Blit( const gxDrawCmd& );
extern void gx_ExecCommand_Line( const gxDrawCmd& );
extern void gx_ExecCommand_Rect( const gxDrawCmd& );
extern void gx_ExecCommand_Tris( const gxDrawCmd& );


void gx_ExecCommand( const gxDrawCmd &cmd )
{
    switch (cmd.type)
    {
        default: kpanic("[gx_ExecCommand] Invalid command type");  break;
        case gxDrawCmd_Blit:      gx_ExecCommand_Blit(cmd);  break;
        case gxDrawCmd_Line:      gx_ExecCommand_Line(cmd);  break;
        case gxDrawCmd_Rect:      gx_ExecCommand_Rect(cmd);  break;
        case gxDrawCmd_Tris:      gx_ExecCommand_Tris(cmd);  break;
    }
}

