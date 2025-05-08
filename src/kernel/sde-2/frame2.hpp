#pragma once

#include <kassert.h>
#include <kdef.h>
#include <kmath/vec.hpp>


enum sdeFrame_: uint32_t
{
    sdeFrame_Invalid = 0,
    sdeFrame_Base,
};

enum sdeDataloc_: uint8_t
{
    sdeDataloc_Footer  = 1,
    sdeDataloc_Pointer = 2,
};


struct sdeFrameStyle
{
    u8vec4 padding;
    u8vec4 margin;
    // u8vec4 
};


struct sdeFrameHeader
{
    uint32_t type;
    uint32_t flags;
    u32vec2  corner;
    u32vec2  extents;
    uint8_t *m_data;
    uint8_t *m_end;

    sdeFrameHeader( uint32_t tp, uint32_t fg, size_t nbytes, void *data )
    :   type(tp), flags(fg)
    {
    
    }
};


struct sdeFrame: sdeFrameHeader
{
    uint8_t footer[];

    sdeFrame( uint32_t fg, void *data, size_t nbytes )
    :   sdeFrameHeader(sdeFrame_Base, fg, nbytes, data)
    {  };

    sdeFrame( uint32_t fg, size_t nbytes )
    :   sdeFrameHeader(sdeFrame_Base, fg, nbytes, footer)
    {  };




};


struct sdeFrameContainer: sdeFrameHeader
{
    uint8_t footer[];
};


struct sdeFrameWindow: sdeFrameHeader
{
    uint8_t footer[];
};


