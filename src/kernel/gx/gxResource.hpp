#pragma once

enum gxResourceStatus_: uint32_t
{
    gxResourceStatus_Invalid = 0,
    gxResourceStatus_Active  = 1,
    gxResourceStatus_Deleted = 2,
};

enum gxResourceFlag_: uint32_t
{
    gxResourceFlag_None        = 0,
    gxResourceFlag_ExplicitAlloc = 1<<0,
    gxResourceFlag_ExplicitFree  = 1<<1,
};

