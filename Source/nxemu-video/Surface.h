#pragma once
#include <stdint.h>
#include "GpuTypes.h"

enum SurfacePixelFormat 
{
    SurfacePixelFormat_A8B8G8R8_UNORM,
    SurfacePixelFormat_A8B8G8R8_SNORM,
    SurfacePixelFormat_A8B8G8R8_SINT,
    SurfacePixelFormat_A8B8G8R8_UINT,
    SurfacePixelFormat_R5G6B5_UNORM,
    SurfacePixelFormat_B5G6R5_UNORM,
    SurfacePixelFormat_A1R5G5B5_UNORM,
    SurfacePixelFormat_A2B10G10R10_UNORM,
    SurfacePixelFormat_A2B10G10R10_UINT,
    SurfacePixelFormat_A1B5G5R5_UNORM,
    SurfacePixelFormat_R8_UNORM,
    SurfacePixelFormat_R8_SNORM,
    SurfacePixelFormat_R8_SINT,
    SurfacePixelFormat_R8_UINT,
    SurfacePixelFormat_R16G16B16A16_FLOAT,
    SurfacePixelFormat_R16G16B16A16_UNORM,
    SurfacePixelFormat_R16G16B16A16_SNORM,
    SurfacePixelFormat_R16G16B16A16_SINT,
    SurfacePixelFormat_R16G16B16A16_UINT,
    SurfacePixelFormat_B10G11R11_FLOAT,
    SurfacePixelFormat_R32G32B32A32_UINT,
    SurfacePixelFormat_BC1_RGBA_UNORM,
    SurfacePixelFormat_BC2_UNORM,
    SurfacePixelFormat_BC3_UNORM,
    SurfacePixelFormat_BC4_UNORM,
    SurfacePixelFormat_BC4_SNORM,
    SurfacePixelFormat_BC5_UNORM,
    SurfacePixelFormat_BC5_SNORM,
    SurfacePixelFormat_BC7_UNORM,
    SurfacePixelFormat_BC6H_UFLOAT,
    SurfacePixelFormat_BC6H_SFLOAT,
    SurfacePixelFormat_ASTC_2D_4X4_UNORM,
    SurfacePixelFormat_B8G8R8A8_UNORM,
    SurfacePixelFormat_R32G32B32A32_FLOAT,
    SurfacePixelFormat_R32G32B32A32_SINT,
    SurfacePixelFormat_R32G32_FLOAT,
    SurfacePixelFormat_R32G32_SINT,
    SurfacePixelFormat_R32_FLOAT,
    SurfacePixelFormat_R16_FLOAT,
    SurfacePixelFormat_R16_UNORM,
    SurfacePixelFormat_R16_SNORM,
    SurfacePixelFormat_R16_UINT,
    SurfacePixelFormat_R16_SINT,
    SurfacePixelFormat_R16G16_UNORM,
    SurfacePixelFormat_R16G16_FLOAT,
    SurfacePixelFormat_R16G16_UINT,
    SurfacePixelFormat_R16G16_SINT,
    SurfacePixelFormat_R16G16_SNORM,
    SurfacePixelFormat_R32G32B32_FLOAT,
    SurfacePixelFormat_A8B8G8R8_SRGB,
    SurfacePixelFormat_R8G8_UNORM,
    SurfacePixelFormat_R8G8_SNORM,
    SurfacePixelFormat_R8G8_SINT,
    SurfacePixelFormat_R8G8_UINT,
    SurfacePixelFormat_R32G32_UINT,
    SurfacePixelFormat_R16G16B16X16_FLOAT,
    SurfacePixelFormat_R32_UINT,
    SurfacePixelFormat_R32_SINT,
    SurfacePixelFormat_ASTC_2D_8X8_UNORM,
    SurfacePixelFormat_ASTC_2D_8X5_UNORM,
    SurfacePixelFormat_ASTC_2D_5X4_UNORM,
    SurfacePixelFormat_B8G8R8A8_SRGB,
    SurfacePixelFormat_BC1_RGBA_SRGB,
    SurfacePixelFormat_BC2_SRGB,
    SurfacePixelFormat_BC3_SRGB,
    SurfacePixelFormat_BC7_SRGB,
    SurfacePixelFormat_A4B4G4R4_UNORM,
    SurfacePixelFormat_ASTC_2D_4X4_SRGB,
    SurfacePixelFormat_ASTC_2D_8X8_SRGB,
    SurfacePixelFormat_ASTC_2D_8X5_SRGB,
    SurfacePixelFormat_ASTC_2D_5X4_SRGB,
    SurfacePixelFormat_ASTC_2D_5X5_UNORM,
    SurfacePixelFormat_ASTC_2D_5X5_SRGB,
    SurfacePixelFormat_ASTC_2D_10X8_UNORM,
    SurfacePixelFormat_ASTC_2D_10X8_SRGB,
    SurfacePixelFormat_ASTC_2D_6X6_UNORM,
    SurfacePixelFormat_ASTC_2D_6X6_SRGB,
    SurfacePixelFormat_ASTC_2D_10X10_UNORM,
    SurfacePixelFormat_ASTC_2D_10X10_SRGB,
    SurfacePixelFormat_ASTC_2D_12X12_UNORM,
    SurfacePixelFormat_ASTC_2D_12X12_SRGB,
    SurfacePixelFormat_ASTC_2D_8X6_UNORM,
    SurfacePixelFormat_ASTC_2D_8X6_SRGB,
    SurfacePixelFormat_ASTC_2D_6X5_UNORM,
    SurfacePixelFormat_ASTC_2D_6X5_SRGB,
    SurfacePixelFormat_E5B9G9R9_FLOAT,
    SurfacePixelFormat_D32_FLOAT,
    SurfacePixelFormat_D16_UNORM,
    SurfacePixelFormat_D24_UNORM_S8_UINT,
    SurfacePixelFormat_S8_UINT_D24_UNORM,
    SurfacePixelFormat_D32_FLOAT_S8_UINT,
    SurfacePixelFormat_MaxColorFormat = SurfacePixelFormat_D32_FLOAT,
    SurfacePixelFormat_MaxDepthFormat = SurfacePixelFormat_D24_UNORM_S8_UINT,
    SurfacePixelFormat_MaxDepthStencilFormat = SurfacePixelFormat_D32_FLOAT_S8_UINT + 1,
    SurfacePixelFormat_MaxPixelFormat = SurfacePixelFormat_MaxDepthStencilFormat,
    SurfacePixelFormat_Invalid = 255,
};

uint32_t SurfacePixelFormatBytesPerBlock(SurfacePixelFormat Format);
SurfacePixelFormat SurfacePixelFormatFromRenderTargetFormat(RenderTargetFormat Format);
uint32_t SurfaceDefaultBlockHeight(SurfacePixelFormat Format);
uint32_t SurfaceDefaultBlockWidth(SurfacePixelFormat Format);