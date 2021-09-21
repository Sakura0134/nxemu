#include "ShaderInstruction.h"
#include "VideoNotification.h"

bool ShaderInstruction::_tyAttribute::_Fmt20::IsPhysical() const
{
    return Patch == 0 && Element == 0 && Index == 0;
}

uint64_t ShaderInstruction::_tyCBuf34::GetOffset() const
{
    return ShiftedOffset * 4;
}

ShaderF2FRoundingOp ShaderInstruction::_tyConversion::_F2F::GetRoundingMode() const
{
    enum { RoundingMask = 0x0B };
    return (ShaderF2FRoundingOp)(Rounding & RoundingMask);
}

bool ShaderInstruction::_tyTexs::IsArrayTexture() const
{
    return TextureInfo >= 7 && TextureInfo <= 9;
}

ShaderTextureProcessMode ShaderInstruction::_tyTexs::GetTextureProcessMode() const 
{
    switch (TextureInfo) 
    {
    case 0:
    case 2:
    case 6:
    case 8:
    case 9:
    case 11:
        return ShaderTextureProcessMode_LZ;
    case 3:
    case 5:
    case 13:
        return ShaderTextureProcessMode_LL;
    case 1:
        return ShaderTextureProcessMode_None;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return ShaderTextureProcessMode_None;
}

ShaderTextureType ShaderInstruction::_tyTexs::GetTextureType() const 
{
    if (TextureInfo == 0) 
    {
        return ShaderTextureType_1D;
    }
    if (TextureInfo >= 1 && TextureInfo <= 9)
    {
        return ShaderTextureType_2D;
    }
    if (TextureInfo >= 10 && TextureInfo <= 11)
    {
        return ShaderTextureType_3D;
    }
    if (TextureInfo >= 12 && TextureInfo <= 13)
    {
        return ShaderTextureType_Cube;
    }

    g_Notify->BreakPoint(__FILE__, __LINE__);
    return ShaderTextureType_1D;
}

bool ShaderInstruction::_tyTexs::IsComponentEnabled(uint32_t Component) const 
{
    static uint32_t MaskLut[4][8] = 
    {
        {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
        {0x1, 0x2, 0x4, 0x8, 0x3, 0x9, 0xa, 0xc},
        {0x1, 0x2, 0x4, 0x8, 0x3, 0x9, 0xa, 0xc},
        {0x7, 0xb, 0xd, 0xe, 0xf, 0x0, 0x0, 0x0},
    };

    uint32_t Index = (Gpr0 != ShaderRegister_ZeroIndex ? 1U : 0U) | (Gpr28 != ShaderRegister_ZeroIndex ? 2 : 0);
    uint32_t Mask = MaskLut[Index][ComponentMaskSelector];
    if (Mask == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return ((1u << Component) & Mask) != 0;
}

bool ShaderInstruction::_tyTexs::HasTwoDestinations() const
{
    return Gpr28 != ShaderRegister_ZeroIndex;
}

bool ShaderInstruction::_tyTexs::UsesMiscMode(ShaderTextureMiscMode Mode) const 
{
    switch (Mode) 
    {
    case ShaderTextureMiscMode_DC: return (TextureInfo >= 4 && TextureInfo <= 6) || TextureInfo == 9;
    case ShaderTextureMiscMode_NODEP: return NodepFlag != 0;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}
