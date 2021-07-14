#pragma once
#include <stdint.h>

enum 
{
    Texture_GOB_SizeX = 64,
    Texture_GOB_SizeY = 8,
    Texture_GOB_SizeZ = 1,

    Texture_GOB_SizeXShift = 6,
    Texture_GOB_SizeYShift = 3,
    Texture_GOB_SizeZShift = 0,
    Texture_GOB_SizeShift = Texture_GOB_SizeXShift + Texture_GOB_SizeYShift + Texture_GOB_SizeZShift,
};

enum MsaaMode
{
    MsaaMode_1x1 = 0,
    MsaaMode_2x1 = 1,
    MsaaMode_2x2 = 2,
    MsaaMode_4x2 = 3,
    MsaaMode_4x2_D3D = 4,
    MsaaMode_2x1_D3D = 5,
    MsaaMode_4x4 = 6,
    MsaaMode_2x2_VC4 = 8,
    MsaaMode_2x2_VC12 = 9,
    MsaaMode_4x2_VC8 = 10,
    MsaaMode_4x2_VC24 = 11,
};

uint32_t MsaaModeSamples(MsaaMode Mode);