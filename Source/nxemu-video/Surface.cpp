#include "Surface.h"
#include "VideoNotification.h"

uint32_t SurfacePixelFormatBytesPerBlock(SurfacePixelFormat PixelFormat) 
{
    switch (PixelFormat) 
    {
    case SurfacePixelFormat_A8B8G8R8_UNORM: return 4;
    case SurfacePixelFormat_A8B8G8R8_SNORM: return 4;
    case SurfacePixelFormat_A8B8G8R8_SINT: return 4;
    case SurfacePixelFormat_A8B8G8R8_UINT: return 4;
    case SurfacePixelFormat_R5G6B5_UNORM: return 2;
    case SurfacePixelFormat_B5G6R5_UNORM: return 2;
    case SurfacePixelFormat_A1R5G5B5_UNORM: return 2; 
    case SurfacePixelFormat_A2B10G10R10_UNORM: return 4;
    case SurfacePixelFormat_A2B10G10R10_UINT: return 4;
    case SurfacePixelFormat_A1B5G5R5_UNORM: return 2;
    case SurfacePixelFormat_R8_UNORM : return 1;
    case SurfacePixelFormat_R8_SNORM: return 1;
    case SurfacePixelFormat_R8_SINT: return 1;
    case SurfacePixelFormat_R8_UINT: return 1;
    case SurfacePixelFormat_R16G16B16A16_FLOAT: return 8;
    case SurfacePixelFormat_R16G16B16A16_UNORM: return 8;
    case SurfacePixelFormat_R16G16B16A16_SNORM: return 8;
    case SurfacePixelFormat_R16G16B16A16_SINT: return 8;
    case SurfacePixelFormat_R16G16B16A16_UINT: return 8;
    case SurfacePixelFormat_B10G11R11_FLOAT: return 4;
    case SurfacePixelFormat_R32G32B32A32_UINT: return 16;
    case SurfacePixelFormat_BC1_RGBA_UNORM: return 8;
    case SurfacePixelFormat_BC2_UNORM: return 16;
    case SurfacePixelFormat_BC3_UNORM: return 16;
    case SurfacePixelFormat_BC4_UNORM: return 8; 
    case SurfacePixelFormat_BC4_SNORM: return 8;
    case SurfacePixelFormat_BC5_UNORM: return 16; 
    case SurfacePixelFormat_BC5_SNORM: return 16;
    case SurfacePixelFormat_BC7_UNORM: return 16;
    case SurfacePixelFormat_BC6H_UFLOAT: return 16;
    case SurfacePixelFormat_BC6H_SFLOAT: return 16;
    case SurfacePixelFormat_ASTC_2D_4X4_UNORM: return 16;
    case SurfacePixelFormat_B8G8R8A8_UNORM: return 4;
    case SurfacePixelFormat_R32G32B32A32_FLOAT: return 16;
    case SurfacePixelFormat_R32G32B32A32_SINT: return 16;
    case SurfacePixelFormat_R32G32_FLOAT: return 8; 
    case SurfacePixelFormat_R32G32_SINT: return 8;
    case SurfacePixelFormat_R32_FLOAT: return 4;
    case SurfacePixelFormat_R16_FLOAT: return 2;
    case SurfacePixelFormat_R16_UNORM: return 2;
    case SurfacePixelFormat_R16_SNORM: return 2;
    case SurfacePixelFormat_R16_UINT: return 2;
    case SurfacePixelFormat_R16_SINT: return 2;
    case SurfacePixelFormat_R16G16_UNORM: return 4; 
    case SurfacePixelFormat_R16G16_FLOAT: return 4;
    case SurfacePixelFormat_R16G16_UINT: return 4;
    case SurfacePixelFormat_R16G16_SINT: return 4; 
    case SurfacePixelFormat_R16G16_SNORM: return 4;
    case SurfacePixelFormat_R32G32B32_FLOAT: return 12;
    case SurfacePixelFormat_A8B8G8R8_SRGB: return 4;
    case SurfacePixelFormat_R8G8_UNORM: return 2;
    case SurfacePixelFormat_R8G8_SNORM: return 2;
    case SurfacePixelFormat_R8G8_SINT: return 2;
    case SurfacePixelFormat_R8G8_UINT: return 2;
    case SurfacePixelFormat_R32G32_UINT: return 8;
    case SurfacePixelFormat_R16G16B16X16_FLOAT: return 8;
    case SurfacePixelFormat_R32_UINT: return 4;
    case SurfacePixelFormat_R32_SINT: return 4;
    case SurfacePixelFormat_ASTC_2D_8X8_UNORM: return 16;
    case SurfacePixelFormat_ASTC_2D_8X5_UNORM: return 16;
    case SurfacePixelFormat_ASTC_2D_5X4_UNORM: return 16;
    case SurfacePixelFormat_B8G8R8A8_SRGB: return 4;
    case SurfacePixelFormat_BC1_RGBA_SRGB: return 8; 
    case SurfacePixelFormat_BC2_SRGB: return 16;
    case SurfacePixelFormat_BC3_SRGB: return 16;
    case SurfacePixelFormat_BC7_SRGB: return 16;
    case SurfacePixelFormat_A4B4G4R4_UNORM: return 2;
    case SurfacePixelFormat_ASTC_2D_4X4_SRGB: return 16;
    case SurfacePixelFormat_ASTC_2D_8X8_SRGB: return 16;
    case SurfacePixelFormat_ASTC_2D_8X5_SRGB: return 16;
    case SurfacePixelFormat_ASTC_2D_5X4_SRGB: return 16;
    case SurfacePixelFormat_ASTC_2D_5X5_UNORM: return 16;
    case SurfacePixelFormat_ASTC_2D_5X5_SRGB: return 16;
    case SurfacePixelFormat_ASTC_2D_10X8_UNORM: return 16;
    case SurfacePixelFormat_ASTC_2D_10X8_SRGB: return 16;
    case SurfacePixelFormat_ASTC_2D_6X6_UNORM: return 16;
    case SurfacePixelFormat_ASTC_2D_6X6_SRGB: return 16;
    case SurfacePixelFormat_ASTC_2D_10X10_UNORM: return 16;
    case SurfacePixelFormat_ASTC_2D_10X10_SRGB: return 16;
    case SurfacePixelFormat_ASTC_2D_12X12_UNORM: return 16;
    case SurfacePixelFormat_ASTC_2D_12X12_SRGB: return 16;
    case SurfacePixelFormat_ASTC_2D_8X6_UNORM: return 16;
    case SurfacePixelFormat_ASTC_2D_8X6_SRGB: return  16;
    case SurfacePixelFormat_ASTC_2D_6X5_UNORM: return 16;
    case SurfacePixelFormat_ASTC_2D_6X5_SRGB: return 16;
    case SurfacePixelFormat_E5B9G9R9_FLOAT: return 4;
    case SurfacePixelFormat_D32_FLOAT: return 4;
    case SurfacePixelFormat_D16_UNORM: return 2;
    case SurfacePixelFormat_D24_UNORM_S8_UINT: return 4;
    case SurfacePixelFormat_S8_UINT_D24_UNORM: return 4;
    case SurfacePixelFormat_D32_FLOAT_S8_UINT: return 8;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 0;
}

bool IsSurfacePixelFormatSRGB(SurfacePixelFormat Format)
{
    switch (Format) 
    {
    case SurfacePixelFormat_A8B8G8R8_SRGB:
    case SurfacePixelFormat_B8G8R8A8_SRGB:
    case SurfacePixelFormat_BC1_RGBA_SRGB:
    case SurfacePixelFormat_BC2_SRGB:
    case SurfacePixelFormat_BC3_SRGB:
    case SurfacePixelFormat_BC7_SRGB:
    case SurfacePixelFormat_ASTC_2D_4X4_SRGB:
    case SurfacePixelFormat_ASTC_2D_8X8_SRGB:
    case SurfacePixelFormat_ASTC_2D_8X5_SRGB:
    case SurfacePixelFormat_ASTC_2D_5X4_SRGB:
    case SurfacePixelFormat_ASTC_2D_5X5_SRGB:
    case SurfacePixelFormat_ASTC_2D_10X8_SRGB:
    case SurfacePixelFormat_ASTC_2D_6X6_SRGB:
    case SurfacePixelFormat_ASTC_2D_10X10_SRGB:
    case SurfacePixelFormat_ASTC_2D_12X12_SRGB:
    case SurfacePixelFormat_ASTC_2D_8X6_SRGB:
    case SurfacePixelFormat_ASTC_2D_6X5_SRGB:
        return true;
    }
    return false;
}

SurfacePixelFormat SurfacePixelFormatFromRenderTargetFormat(RenderTargetFormat Format) 
{
    switch (Format)
    {
    case RenderTargetFormat_R32B32G32A32_FLOAT: return SurfacePixelFormat_R32G32B32A32_FLOAT;
    case RenderTargetFormat_R32G32B32A32_SINT: return SurfacePixelFormat_R32G32B32A32_SINT;
    case RenderTargetFormat_R32G32B32A32_UINT: return SurfacePixelFormat_R32G32B32A32_UINT;
    case RenderTargetFormat_R16G16B16A16_UNORM: return SurfacePixelFormat_R16G16B16A16_UNORM;
    case RenderTargetFormat_R16G16B16A16_SNORM: return SurfacePixelFormat_R16G16B16A16_SNORM;
    case RenderTargetFormat_R16G16B16A16_SINT: return SurfacePixelFormat_R16G16B16A16_SINT;
    case RenderTargetFormat_R16G16B16A16_UINT: return SurfacePixelFormat_R16G16B16A16_UINT;
    case RenderTargetFormat_R16G16B16A16_FLOAT: return SurfacePixelFormat_R16G16B16A16_FLOAT;
    case RenderTargetFormat_R32G32_FLOAT: return SurfacePixelFormat_R32G32_FLOAT;
    case RenderTargetFormat_R32G32_SINT: return SurfacePixelFormat_R32G32_SINT;
    case RenderTargetFormat_R32G32_UINT: return SurfacePixelFormat_R32G32_UINT;
    case RenderTargetFormat_R16G16B16X16_FLOAT: return SurfacePixelFormat_R16G16B16X16_FLOAT;
    case RenderTargetFormat_B8G8R8A8_UNORM: return SurfacePixelFormat_B8G8R8A8_UNORM;
    case RenderTargetFormat_B8G8R8A8_SRGB: return SurfacePixelFormat_B8G8R8A8_SRGB;
    case RenderTargetFormat_A2B10G10R10_UNORM: return SurfacePixelFormat_A2B10G10R10_UNORM;
    case RenderTargetFormat_A2B10G10R10_UINT: return SurfacePixelFormat_A2B10G10R10_UINT;
    case RenderTargetFormat_A8B8G8R8_UNORM: return SurfacePixelFormat_A8B8G8R8_UNORM;
    case RenderTargetFormat_A8B8G8R8_SRGB: return SurfacePixelFormat_A8B8G8R8_SRGB;
    case RenderTargetFormat_A8B8G8R8_SNORM: return SurfacePixelFormat_A8B8G8R8_SNORM;
    case RenderTargetFormat_A8B8G8R8_SINT: return SurfacePixelFormat_A8B8G8R8_SINT;
    case RenderTargetFormat_A8B8G8R8_UINT: return SurfacePixelFormat_A8B8G8R8_UINT;
    case RenderTargetFormat_R16G16_UNORM: return SurfacePixelFormat_R16G16_UNORM;
    case RenderTargetFormat_R16G16_SNORM: return SurfacePixelFormat_R16G16_SNORM;
    case RenderTargetFormat_R16G16_SINT: return SurfacePixelFormat_R16G16_SINT;
    case RenderTargetFormat_R16G16_UINT: return SurfacePixelFormat_R16G16_UINT;
    case RenderTargetFormat_R16G16_FLOAT: return SurfacePixelFormat_R16G16_FLOAT;
    case RenderTargetFormat_B10G11R11_FLOAT: return SurfacePixelFormat_B10G11R11_FLOAT;
    case RenderTargetFormat_R32_SINT: return SurfacePixelFormat_R32_SINT;
    case RenderTargetFormat_R32_UINT: return SurfacePixelFormat_R32_UINT;
    case RenderTargetFormat_R32_FLOAT: return SurfacePixelFormat_R32_FLOAT;
    case RenderTargetFormat_R5G6B5_UNORM: return SurfacePixelFormat_R5G6B5_UNORM;
    case RenderTargetFormat_A1R5G5B5_UNORM: return SurfacePixelFormat_A1R5G5B5_UNORM;
    case RenderTargetFormat_R8G8_UNORM: return SurfacePixelFormat_R8G8_UNORM;
    case RenderTargetFormat_R8G8_SNORM: return SurfacePixelFormat_R8G8_SNORM;
    case RenderTargetFormat_R8G8_SINT: return SurfacePixelFormat_R8G8_SINT;
    case RenderTargetFormat_R8G8_UINT: return SurfacePixelFormat_R8G8_UINT;
    case RenderTargetFormat_R16_UNORM: return SurfacePixelFormat_R16_UNORM;
    case RenderTargetFormat_R16_SNORM: return SurfacePixelFormat_R16_SNORM;
    case RenderTargetFormat_R16_SINT: return SurfacePixelFormat_R16_SINT;
    case RenderTargetFormat_R16_UINT: return SurfacePixelFormat_R16_UINT;
    case RenderTargetFormat_R16_FLOAT: return SurfacePixelFormat_R16_FLOAT;
    case RenderTargetFormat_R8_UNORM: return SurfacePixelFormat_R8_UNORM;
    case RenderTargetFormat_R8_SNORM: return SurfacePixelFormat_R8_SNORM;
    case RenderTargetFormat_R8_SINT: return SurfacePixelFormat_R8_SINT;
    case RenderTargetFormat_R8_UINT: return SurfacePixelFormat_R8_UINT;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return SurfacePixelFormat_A8B8G8R8_UNORM;
}

uint32_t SurfaceDefaultBlockHeight(SurfacePixelFormat Format) 
{
    switch (Format) 
    {
    case SurfacePixelFormat_A8B8G8R8_UNORM: return 1; 
    case SurfacePixelFormat_A8B8G8R8_SNORM: return 1;
    case SurfacePixelFormat_A8B8G8R8_SINT: return 1;
    case SurfacePixelFormat_A8B8G8R8_UINT: return 1;
    case SurfacePixelFormat_R5G6B5_UNORM: return 1;
    case SurfacePixelFormat_B5G6R5_UNORM: return 1;
    case SurfacePixelFormat_A1R5G5B5_UNORM: return 1;
    case SurfacePixelFormat_A2B10G10R10_UNORM: return 1;
    case SurfacePixelFormat_A2B10G10R10_UINT: return 1;
    case SurfacePixelFormat_A1B5G5R5_UNORM: return 1;
    case SurfacePixelFormat_R8_UNORM: return 1;
    case SurfacePixelFormat_R8_SNORM: return 1;
    case SurfacePixelFormat_R8_SINT: return 1; 
    case SurfacePixelFormat_R8_UINT: return 1;
    case SurfacePixelFormat_R16G16B16A16_FLOAT: return 1;
    case SurfacePixelFormat_R16G16B16A16_UNORM: return 1;
    case SurfacePixelFormat_R16G16B16A16_SNORM: return 1;
    case SurfacePixelFormat_R16G16B16A16_SINT: return 1;
    case SurfacePixelFormat_R16G16B16A16_UINT: return 1;
    case SurfacePixelFormat_B10G11R11_FLOAT: return 1;
    case SurfacePixelFormat_R32G32B32A32_UINT: return 1;
    case SurfacePixelFormat_BC1_RGBA_UNORM: return 4;
    case SurfacePixelFormat_BC2_UNORM: return 4;
    case SurfacePixelFormat_BC3_UNORM: return 4;
    case SurfacePixelFormat_BC4_UNORM: return 4;
    case SurfacePixelFormat_BC4_SNORM: return 4;
    case SurfacePixelFormat_BC5_UNORM: return 4;
    case SurfacePixelFormat_BC5_SNORM: return 4;
    case SurfacePixelFormat_BC7_UNORM: return 4;
    case SurfacePixelFormat_BC6H_UFLOAT: return 4;
    case SurfacePixelFormat_BC6H_SFLOAT: return 4;
    case SurfacePixelFormat_ASTC_2D_4X4_UNORM: return 4;
    case SurfacePixelFormat_B8G8R8A8_UNORM: return 1;
    case SurfacePixelFormat_R32G32B32A32_FLOAT: return 1;
    case SurfacePixelFormat_R32G32B32A32_SINT: return 1;
    case SurfacePixelFormat_R32G32_FLOAT: return 1;
    case SurfacePixelFormat_R32G32_SINT: return 1;
    case SurfacePixelFormat_R32_FLOAT: return 1;
    case SurfacePixelFormat_R16_FLOAT: return 1;
    case SurfacePixelFormat_R16_UNORM: return 1;
    case SurfacePixelFormat_R16_SNORM: return 1;
    case SurfacePixelFormat_R16_UINT: return 1;
    case SurfacePixelFormat_R16_SINT: return 1;
    case SurfacePixelFormat_R16G16_UNORM: return 1;
    case SurfacePixelFormat_R16G16_FLOAT: return 1;
    case SurfacePixelFormat_R16G16_UINT: return 1;
    case SurfacePixelFormat_R16G16_SINT: return 1;
    case SurfacePixelFormat_R16G16_SNORM: return 1;
    case SurfacePixelFormat_R32G32B32_FLOAT: return 1;
    case SurfacePixelFormat_A8B8G8R8_SRGB: return 1;
    case SurfacePixelFormat_R8G8_UNORM: return 1;
    case SurfacePixelFormat_R8G8_SNORM: return 1;
    case SurfacePixelFormat_R8G8_SINT: return 1;
    case SurfacePixelFormat_R8G8_UINT: return 1;
    case SurfacePixelFormat_R32G32_UINT: return 1;
    case SurfacePixelFormat_R16G16B16X16_FLOAT: return 1;
    case SurfacePixelFormat_R32_UINT: return 1;
    case SurfacePixelFormat_R32_SINT: return 1;
    case SurfacePixelFormat_ASTC_2D_8X8_UNORM: return 8;
    case SurfacePixelFormat_ASTC_2D_8X5_UNORM: return 5;
    case SurfacePixelFormat_ASTC_2D_5X4_UNORM: return 4;
    case SurfacePixelFormat_B8G8R8A8_SRGB: return 1;
    case SurfacePixelFormat_BC1_RGBA_SRGB: return 4;
    case SurfacePixelFormat_BC2_SRGB: return 4;
    case SurfacePixelFormat_BC3_SRGB: return 4;
    case SurfacePixelFormat_BC7_SRGB: return 4;
    case SurfacePixelFormat_A4B4G4R4_UNORM: return 1;
    case SurfacePixelFormat_ASTC_2D_4X4_SRGB: return 4;
    case SurfacePixelFormat_ASTC_2D_8X8_SRGB: return 8;
    case SurfacePixelFormat_ASTC_2D_8X5_SRGB: return 5;
    case SurfacePixelFormat_ASTC_2D_5X4_SRGB: return 4;
    case SurfacePixelFormat_ASTC_2D_5X5_UNORM: return 5;
    case SurfacePixelFormat_ASTC_2D_5X5_SRGB: return 5;
    case SurfacePixelFormat_ASTC_2D_10X8_UNORM: return 8;
    case SurfacePixelFormat_ASTC_2D_10X8_SRGB: return 8;
    case SurfacePixelFormat_ASTC_2D_6X6_UNORM: return 6;
    case SurfacePixelFormat_ASTC_2D_6X6_SRGB: return 6;
    case SurfacePixelFormat_ASTC_2D_10X10_UNORM: return 10;
    case SurfacePixelFormat_ASTC_2D_10X10_SRGB: return 10;
    case SurfacePixelFormat_ASTC_2D_12X12_UNORM: return 12;
    case SurfacePixelFormat_ASTC_2D_12X12_SRGB: return 12;
    case SurfacePixelFormat_ASTC_2D_8X6_UNORM: return 6;
    case SurfacePixelFormat_ASTC_2D_8X6_SRGB: return 6;
    case SurfacePixelFormat_ASTC_2D_6X5_UNORM: return 5;
    case SurfacePixelFormat_ASTC_2D_6X5_SRGB: return 5;
    case SurfacePixelFormat_E5B9G9R9_FLOAT: return 1;
    case SurfacePixelFormat_D32_FLOAT: return 1;
    case SurfacePixelFormat_D16_UNORM: return 1;
    case SurfacePixelFormat_D24_UNORM_S8_UINT: return 1;
    case SurfacePixelFormat_S8_UINT_D24_UNORM: return 1;
    case SurfacePixelFormat_D32_FLOAT_S8_UINT: return 1;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 1;
}

uint32_t SurfaceDefaultBlockWidth(SurfacePixelFormat Format) 
{
    switch (Format) 
    {
    case SurfacePixelFormat_A8B8G8R8_UNORM: return 1; 
    case SurfacePixelFormat_A8B8G8R8_SNORM: return 1;
    case SurfacePixelFormat_A8B8G8R8_SINT: return 1; 
    case SurfacePixelFormat_A8B8G8R8_UINT: return 1;
    case SurfacePixelFormat_R5G6B5_UNORM: return 1; 
    case SurfacePixelFormat_B5G6R5_UNORM: return 1;
    case SurfacePixelFormat_A1R5G5B5_UNORM: return 1;
    case SurfacePixelFormat_A2B10G10R10_UNORM: return 1;
    case SurfacePixelFormat_A2B10G10R10_UINT: return 1;
    case SurfacePixelFormat_A1B5G5R5_UNORM: return 1;
    case SurfacePixelFormat_R8_UNORM: return 1;
    case SurfacePixelFormat_R8_SNORM: return 1;
    case SurfacePixelFormat_R8_SINT: return 1;
    case SurfacePixelFormat_R8_UINT: return 1;
    case SurfacePixelFormat_R16G16B16A16_FLOAT: return 1;
    case SurfacePixelFormat_R16G16B16A16_UNORM: return 1;
    case SurfacePixelFormat_R16G16B16A16_SNORM: return 1;
    case SurfacePixelFormat_R16G16B16A16_SINT: return 1;
    case SurfacePixelFormat_R16G16B16A16_UINT: return 1;
    case SurfacePixelFormat_B10G11R11_FLOAT: return 1;
    case SurfacePixelFormat_R32G32B32A32_UINT: return 1;
    case SurfacePixelFormat_BC1_RGBA_UNORM: return 4;
    case SurfacePixelFormat_BC2_UNORM: return 4;
    case SurfacePixelFormat_BC3_UNORM: return 4;
    case SurfacePixelFormat_BC4_UNORM: return 4;
    case SurfacePixelFormat_BC4_SNORM: return 4;
    case SurfacePixelFormat_BC5_UNORM: return 4;
    case SurfacePixelFormat_BC5_SNORM: return 4;
    case SurfacePixelFormat_BC7_UNORM: return 4;
    case SurfacePixelFormat_BC6H_UFLOAT: return 4;
    case SurfacePixelFormat_BC6H_SFLOAT: return 4;
    case SurfacePixelFormat_ASTC_2D_4X4_UNORM: return 4;
    case SurfacePixelFormat_B8G8R8A8_UNORM: return 1;
    case SurfacePixelFormat_R32G32B32A32_FLOAT: return 1;
    case SurfacePixelFormat_R32G32B32A32_SINT: return 1;
    case SurfacePixelFormat_R32G32_FLOAT: return 1;
    case SurfacePixelFormat_R32G32_SINT: return 1;
    case SurfacePixelFormat_R32_FLOAT: return 1;
    case SurfacePixelFormat_R16_FLOAT: return 1;
    case SurfacePixelFormat_R16_UNORM: return 1;
    case SurfacePixelFormat_R16_SNORM: return 1;
    case SurfacePixelFormat_R16_UINT: return 1;
    case SurfacePixelFormat_R16_SINT: return 1;
    case SurfacePixelFormat_R16G16_UNORM: return 1;
    case SurfacePixelFormat_R16G16_FLOAT: return 1;
    case SurfacePixelFormat_R16G16_UINT: return 1;
    case SurfacePixelFormat_R16G16_SINT: return 1;
    case SurfacePixelFormat_R16G16_SNORM: return 1;
    case SurfacePixelFormat_R32G32B32_FLOAT: return 1;
    case SurfacePixelFormat_A8B8G8R8_SRGB: return 1;
    case SurfacePixelFormat_R8G8_UNORM: return 1;
    case SurfacePixelFormat_R8G8_SNORM: return 1;
    case SurfacePixelFormat_R8G8_SINT: return 1;
    case SurfacePixelFormat_R8G8_UINT: return 1;
    case SurfacePixelFormat_R32G32_UINT: return 1;
    case SurfacePixelFormat_R16G16B16X16_FLOAT: return 1; 
    case SurfacePixelFormat_R32_UINT: return 1;
    case SurfacePixelFormat_R32_SINT: return 1; 
    case SurfacePixelFormat_ASTC_2D_8X8_UNORM: return 8;
    case SurfacePixelFormat_ASTC_2D_8X5_UNORM: return 8;
    case SurfacePixelFormat_ASTC_2D_5X4_UNORM: return 5;
    case SurfacePixelFormat_B8G8R8A8_SRGB: return 1;
    case SurfacePixelFormat_BC1_RGBA_SRGB: return 4;
    case SurfacePixelFormat_BC2_SRGB: return 4; 
    case SurfacePixelFormat_BC3_SRGB: return 4;
    case SurfacePixelFormat_BC7_SRGB: return 4; 
    case SurfacePixelFormat_A4B4G4R4_UNORM: return 1; 
    case SurfacePixelFormat_ASTC_2D_4X4_SRGB: return 4;
    case SurfacePixelFormat_ASTC_2D_8X8_SRGB: return 8; 
    case SurfacePixelFormat_ASTC_2D_8X5_SRGB: return 8;
    case SurfacePixelFormat_ASTC_2D_5X4_SRGB: return 5;
    case SurfacePixelFormat_ASTC_2D_5X5_UNORM: return 5;
    case SurfacePixelFormat_ASTC_2D_5X5_SRGB: return 5; 
    case SurfacePixelFormat_ASTC_2D_10X8_UNORM: return 10;
    case SurfacePixelFormat_ASTC_2D_10X8_SRGB: return 10; 
    case SurfacePixelFormat_ASTC_2D_6X6_UNORM: return 6;
    case SurfacePixelFormat_ASTC_2D_6X6_SRGB: return 6;
    case SurfacePixelFormat_ASTC_2D_10X10_UNORM: return 10;
    case SurfacePixelFormat_ASTC_2D_10X10_SRGB: return 10;
    case SurfacePixelFormat_ASTC_2D_12X12_UNORM: return 12;
    case SurfacePixelFormat_ASTC_2D_12X12_SRGB: return 12;
    case SurfacePixelFormat_ASTC_2D_8X6_UNORM: return 8;
    case SurfacePixelFormat_ASTC_2D_8X6_SRGB: return 8;
    case SurfacePixelFormat_ASTC_2D_6X5_UNORM: return 6;
    case SurfacePixelFormat_ASTC_2D_6X5_SRGB: return 6;
    case SurfacePixelFormat_E5B9G9R9_FLOAT: return 1;
    case SurfacePixelFormat_D32_FLOAT: return 1;
    case SurfacePixelFormat_D16_UNORM: return 1;
    case SurfacePixelFormat_D24_UNORM_S8_UINT: return 1;
    case SurfacePixelFormat_S8_UINT_D24_UNORM: return 1;
    case SurfacePixelFormat_D32_FLOAT_S8_UINT: return 1;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return 1;
}
