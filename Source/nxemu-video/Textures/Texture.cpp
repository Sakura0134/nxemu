#include "Texture.h"
#include "VideoNotification.h"

uint32_t MsaaModeSamples(MsaaMode Mode) 
{
    switch (Mode)
    {
    case MsaaMode_1x1:
        return 1;
    case MsaaMode_2x1:
    case MsaaMode_2x1_D3D:
        return 2;
    case MsaaMode_2x2:
    case MsaaMode_2x2_VC4:
    case MsaaMode_2x2_VC12:
        return 4;
    case MsaaMode_4x2:
    case MsaaMode_4x2_D3D:
    case MsaaMode_4x2_VC8:
    case MsaaMode_4x2_VC24:
        return 8;
    case MsaaMode_4x4:
        return 16;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 1;
}
