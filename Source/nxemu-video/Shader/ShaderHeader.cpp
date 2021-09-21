#include "ShaderHeader.h"
#include "VideoNotification.h"

ShaderPixelImap ShaderHeader::GetPixelImap(uint32_t Attribute) const 
{
    ShaderPixelImap Values[] = 
    {
        PS.ImapGenericVector[Attribute].ImapX,
        PS.ImapGenericVector[Attribute].ImapY,
        PS.ImapGenericVector[Attribute].ImapZ,
        PS.ImapGenericVector[Attribute].ImapW,
    };

    ShaderPixelImap Result = ShaderPixelImap_Unused;
    for (uint32_t i = 0, n = sizeof(Values) / sizeof(Values[0]); i < n; i++)
    {
        if (Values[i] == ShaderPixelImap_Unused)
        {
            continue;
        }
        if (Result != ShaderPixelImap_Unused && Result != Values[i])
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            break;
        }
        Result = Values[i];
    }
    return Result;
}
