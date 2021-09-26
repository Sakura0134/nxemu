#pragma once
#include "Surface.h"
#include <stdint.h>
#include <functional>

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

enum TextureFormat 
{
    TextureFormat_B5G6R5 = 0x15,
};

enum TextureType : unsigned 
{
    TextureType_1D = 0,
    TextureType_2D = 1,
    TextureType_3D = 2,
    TextureType_Cubemap = 3,
    TextureType_1DArray = 4,
    TextureType_2DArray = 5,
    TextureType_1DBuffer = 6,
    TextureType_2DNoMipmap = 7,
    TextureType_CubeArray = 8,
};

enum TextureTICHeaderVersion : unsigned
{
    TextureTICHeaderVersion_OneDBuffer = 0,
    TextureTICHeaderVersion_PitchColorKey = 1,
    TextureTICHeaderVersion_Pitch = 2,
    TextureTICHeaderVersion_BlockLinear = 3,
    TextureTICHeaderVersion_BlockLinearColorKey = 4,
};

enum TextureComponentType 
{
    TextureComponentType_UNORM = 2,
};

enum TextureSwizzleSource : unsigned 
{
    TextureSwizzleSource_Zero = 0,
    TextureSwizzleSource_R = 2,
    TextureSwizzleSource_G = 3,
    TextureSwizzleSource_B = 4,
    TextureSwizzleSource_A = 5,
    TextureSwizzleSource_OneInt = 6,
    TextureSwizzleSource_OneFloat = 7,
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

union TextureHandle
{
    uint32_t Value;
#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
    struct 
    {
        unsigned TICId : 20;
        unsigned TSCId : 12;
    };
#pragma warning(pop)
};

union TextureTICEntry
{
#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
    struct
    {
        TextureFormat Format : 7;
        TextureComponentType RType : 3;
        TextureComponentType GType : 3;
        TextureComponentType BType : 3;
        TextureComponentType AType : 3;
        TextureSwizzleSource XSource : 3;
        TextureSwizzleSource YSource : 3;
        TextureSwizzleSource ZSource : 3;
        TextureSwizzleSource WSource : 3;
        unsigned : 1;
        
        uint32_t AddressLow;
        
        unsigned AddressHigh : 16;
        unsigned LayerBase_3_7 : 5;
        TextureTICHeaderVersion HeaderVersion : 3;
        unsigned : 5;
        unsigned LayerBase_8_10 : 3;
        union 
        {
            struct 
            {
                unsigned : 10;
                unsigned TileWidthSpacing : 3;
                unsigned : 15;
                unsigned MaxMipLevel : 1;
                unsigned : 3;
            };

            struct 
            {
                unsigned PitchHigh : 16;
                unsigned : 16;
            };
        };
        unsigned WidthMinusOne : 16;
        unsigned LayerBase_0_2 : 3;
        unsigned : 3;
        unsigned SRGBConversion : 1;
        TextureType TextureType : 4;
        unsigned : 5;

        unsigned HeightMinusOne : 16;
        unsigned DepthMinusOne : 14;
        unsigned : 1;
        unsigned NormalizedCoords : 1;

        unsigned : 32;
        
        unsigned ResMinMipLevel : 4;
        unsigned ResMaxMipLevel : 4;
        MsaaMode MsaaMode : 4;
        unsigned : 20;
    };
#pragma warning(pop)
    uint64_t Value[4];

    bool operator==(const TextureTICEntry& rhs) const;
    bool operator!=(const TextureTICEntry& rhs) const;

    uint64_t Address() const;
    uint32_t Pitch() const;
    uint32_t Width() const;
    uint32_t Height() const;
    uint32_t Depth() const;
    uint32_t BaseLayer() const;
    SurfacePixelFormat PixelFormat() const;
    bool IsBlockLinear() const;
    bool IsPitchLinear() const;
};

template <>
struct std::hash<TextureTICEntry>
{
    size_t operator()(const TextureTICEntry& tic) const noexcept;
};

enum TextureWrapMode : unsigned
{
    TextureWrapMode_Wrap = 0,
    TextureWrapMode_Mirror = 1,
    TextureWrapMode_ClampToEdge = 2,
    TextureWrapMode_Border = 3,
    TextureWrapMode_Clamp = 4,
    TextureWrapMode_MirrorOnceClampToEdge = 5,
    TextureWrapMode_MirrorOnceBorder = 6,
    TextureWrapMode_MirrorOnceClampOGL = 7,
};

enum TextureDepthCompareFunc : uint32_t
{
    TextureDepthCompareFunc_Never = 0,
    TextureDepthCompareFunc_Less = 1,
    TextureDepthCompareFunc_Equal = 2,
    TextureDepthCompareFunc_LessEqual = 3,
    TextureDepthCompareFunc_Greater = 4,
    TextureDepthCompareFunc_NotEqual = 5,
    TextureDepthCompareFunc_GreaterEqual = 6,
    TextureDepthCompareFunc_Always = 7,
};

enum TextureFilter : unsigned 
{
    TextureFilter_Nearest = 1,
    TextureFilter_Linear = 2,
};

enum TextureMipmapFilter : unsigned 
{
    TextureMipmapFilter_None = 1,
    TextureMipmapFilter_Nearest = 2,
    TextureMipmapFilter_Linear = 3,
};

enum TextureSamplerReduction : uint32_t
{
    TextureSamplerReduction_WeightedAverage = 0,
    TextureSamplerReduction_Min = 1,
    TextureSamplerReduction_Max = 2,
};

union TextureTSCEntry
{
#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
    struct
    {
        TextureWrapMode WrapU : 3;
        TextureWrapMode WrapV : 3;
        TextureWrapMode WrapP : 3;
        unsigned DepthCompareEnabled : 1;
        TextureDepthCompareFunc DepthCompareFunc : 3;
        unsigned SRGBConversion : 1;
        unsigned : 18;

        TextureFilter MagFilter : 2;
        unsigned : 2;
        TextureFilter MinFilter : 2;
        TextureMipmapFilter MipmapFilter : 2;
        unsigned CubemapAnisotropy : 1;
        unsigned CubemapInterfaceFiltering : 1;
        TextureSamplerReduction ReductionFilter : 2;
        unsigned MipLodBias : 13;
        unsigned FloatCoordNormalization : 1;
        unsigned : 6;

        unsigned MinLodClamp : 12;
        unsigned MaxLodClamp : 12;
        unsigned SRGBBorderColorR : 8;

        unsigned : 12;
        unsigned SRGBBorderColorG : 8;
        unsigned SRGBBorderColorB : 8;
        unsigned : 4;

        float BorderColorValue[4];
    };
#pragma warning(pop)
    uint64_t Value[4];
    bool operator == (const TextureTSCEntry & rhs) const;
    bool operator != (const TextureTSCEntry & rhs) const;
    const float * BorderColor() const;
    float MinLod() const;
    float MaxLod() const;
    float LodBias() const;
};

template <>
struct std::hash<TextureTSCEntry> 
{
    size_t operator()(const TextureTSCEntry& tsc) const noexcept;
};
