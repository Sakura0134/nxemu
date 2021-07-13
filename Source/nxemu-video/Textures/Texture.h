#pragma once
#include <stdint.h>

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
