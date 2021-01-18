#pragma once
#include <Common\stdtypes.h>

#pragma warning(push)
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union

union nvIoctl 
{
    enum
    {
        NvHostCustomMagic = 0x00,
        NvMapCustomMagic = 0x01,
        NvGpuAsMagic = 0x41,
        NvGpuMagic = 0x47,
        NvHostMagic = 0x48,
    };

    uint32_t Raw;
    struct
    {
        unsigned Cmd : 8;
        unsigned Group : 8;
        unsigned Length : 14;
        unsigned In : 1;
        unsigned Out : 1;
    };
};

#pragma warning(pop)
