#include <driver/interface.hpp>


const char *ModuleTypeStr( uint64_t type )
{
    switch (type)
    {
        default: return "INVALID_TYPE";
        case ModuleType_Invalid: return "ModuleType_Invalid";
        case ModuleType_Device: return "DeviceInterface";
        case ModuleType_Daemon: return "DaemonInterface";
    }
}

const char *DeviceTypeStr( uint64_t type )
{
    switch (type)
    {
        default: return "INVALID_TYPE";
        case DeviceType_Invalid: return "DeviceType_Invalid";
        case DeviceType_Storage: return "DeviceType_Storage";
        case DeviceType_Audio: return "DeviceType_Audio";
        case DeviceType_Video: return "DeviceType_Video";
        case DeviceType_Keyboard: return "DeviceType_Keyboard";
        case DeviceType_Mouse: return "DeviceType_Mouse";
        case DeviceType_Block: return "BlockDeviceInterface";
        case DeviceType_Char: return  "CharDeviceInterface";
    }
}

const char *DaemonTypeStr( uint64_t type )
{
    switch (type)
    {
        default: return "INVALID_TYPE";
        case DaemonType_Invalid: return "DaemonType_Invalid";
        case DaemonType_System: return "DaemonType_System";
        case DaemonType_User: return "DaemonType_User";
    }
}


const char *ModuleBaseTypeStr( uint64_t modtype, uint64_t basetype )
{
    switch (modtype)
    {
        default: return "INVALID_TYPE";
        case ModuleType_Device: return DeviceTypeStr(basetype);
        case ModuleType_Daemon: return DaemonTypeStr(basetype);
    }
}


const char *ModuleSubTypeStr( uint64_t modtype, uint64_t subtype )
{
    switch (modtype)
    {
        default: return "INVALID_TYPE";
        case ModuleType_Device: return DeviceTypeStr(subtype);
        case ModuleType_Daemon: return DaemonTypeStr(subtype);
    }
}
