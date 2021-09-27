#pragma once

enum 
{
    MaxShaderTypes = 6,
    NumRenderTargets = 8,
};

enum BufferMethods : unsigned 
{
    BufferMethods_BindObject = 0x0,
    BufferMethods_Nop = 0x2,
    BufferMethods_SemaphoreAddressHigh = 0x4,
    BufferMethods_SemaphoreAddressLow = 0x5,
    BufferMethods_SemaphoreSequence = 0x6,
    BufferMethods_SemaphoreTrigger = 0x7,
    BufferMethods_NotifyIntr = 0x8,
    BufferMethods_WrcacheFlush = 0x9,
    BufferMethods_Unk28 = 0xA,
    BufferMethods_UnkCacheFlush = 0xB,
    BufferMethods_RefCnt = 0x14,
    BufferMethods_SemaphoreAcquire = 0x1A,
    BufferMethods_SemaphoreRelease = 0x1B,
    BufferMethods_FenceValue = 0x1C,
    BufferMethods_FenceAction = 0x1D,
    BufferMethods_WaitForInterrupt = 0x1E,
    BufferMethods_Unk7c = 0x1F,
    BufferMethods_Yield = 0x20,
    BufferMethods_NonPuller = 0x40,
};

enum EngineID 
{
    EngineID_FERMI_TWOD_A = 0x902D,
    EngineID_MAXWELL_B = 0xB197,
    EngineID_KEPLER_COMPUTE_B = 0xB1C0,
    EngineID_KEPLER_INLINE_TO_MEMORY_B = 0xA140,
    EngineID_MAXWELL_DMA_COPY_A = 0xB0B5,
};

enum GPUPixelFormat
{
    GPUPixelFormat_A8B8G8R8_UNORM = 1,
    GPUPixelFormat_RGB565_UNORM = 4,
    GPUPixelFormat_B8G8R8A8_UNORM = 5,
};

enum TransformFlags 
{
    TransformFlags_Unset = 0x00,
    TransformFlags_FlipH = 0x01,
    TransformFlags_FlipV = 0x02,
    TransformFlags_Rotate90 = 0x04,
    TransformFlags_Rotate180 = 0x03,
    TransformFlags_Rotate270 = 0x07,
};

enum RenderTargetFormat 
{
    RenderTargetFormat_NONE = 0x0,
    RenderTargetFormat_R32B32G32A32_FLOAT = 0xC0,
    RenderTargetFormat_R32G32B32A32_SINT = 0xC1,
    RenderTargetFormat_R32G32B32A32_UINT = 0xC2,
    RenderTargetFormat_R16G16B16A16_UNORM = 0xC6,
    RenderTargetFormat_R16G16B16A16_SNORM = 0xC7,
    RenderTargetFormat_R16G16B16A16_SINT = 0xC8,
    RenderTargetFormat_R16G16B16A16_UINT = 0xC9,
    RenderTargetFormat_R16G16B16A16_FLOAT = 0xCA,
    RenderTargetFormat_R32G32_FLOAT = 0xCB,
    RenderTargetFormat_R32G32_SINT = 0xCC,
    RenderTargetFormat_R32G32_UINT = 0xCD,
    RenderTargetFormat_R16G16B16X16_FLOAT = 0xCE,
    RenderTargetFormat_B8G8R8A8_UNORM = 0xCF,
    RenderTargetFormat_B8G8R8A8_SRGB = 0xD0,
    RenderTargetFormat_A2B10G10R10_UNORM = 0xD1,
    RenderTargetFormat_A2B10G10R10_UINT = 0xD2,
    RenderTargetFormat_A8B8G8R8_UNORM = 0xD5,
    RenderTargetFormat_A8B8G8R8_SRGB = 0xD6,
    RenderTargetFormat_A8B8G8R8_SNORM = 0xD7,
    RenderTargetFormat_A8B8G8R8_SINT = 0xD8,
    RenderTargetFormat_A8B8G8R8_UINT = 0xD9,
    RenderTargetFormat_R16G16_UNORM = 0xDA,
    RenderTargetFormat_R16G16_SNORM = 0xDB,
    RenderTargetFormat_R16G16_SINT = 0xDC,
    RenderTargetFormat_R16G16_UINT = 0xDD,
    RenderTargetFormat_R16G16_FLOAT = 0xDE,
    RenderTargetFormat_B10G11R11_FLOAT = 0xE0,
    RenderTargetFormat_R32_SINT = 0xE3,
    RenderTargetFormat_R32_UINT = 0xE4,
    RenderTargetFormat_R32_FLOAT = 0xE5,
    RenderTargetFormat_R5G6B5_UNORM = 0xE8,
    RenderTargetFormat_A1R5G5B5_UNORM = 0xE9,
    RenderTargetFormat_R8G8_UNORM = 0xEA,
    RenderTargetFormat_R8G8_SNORM = 0xEB,
    RenderTargetFormat_R8G8_SINT = 0xEC,
    RenderTargetFormat_R8G8_UINT = 0xED,
    RenderTargetFormat_R16_UNORM = 0xEE,
    RenderTargetFormat_R16_SNORM = 0xEF,
    RenderTargetFormat_R16_SINT = 0xF0,
    RenderTargetFormat_R16_UINT = 0xF1,
    RenderTargetFormat_R16_FLOAT = 0xF2,
    RenderTargetFormat_R8_UNORM = 0xF3,
    RenderTargetFormat_R8_SNORM = 0xF4,
    RenderTargetFormat_R8_SINT = 0xF5,
    RenderTargetFormat_R8_UINT = 0xF6,
};

enum DepthFormat
{
    DepthFormat_D32_FLOAT = 0xA,
    DepthFormat_D16_UNORM = 0x13,
    DepthFormat_S8_UINT_Z24_UNORM = 0x14,
    DepthFormat_D24X8_UNORM = 0x15,
    DepthFormat_D24S8_UNORM = 0x16,
    DepthFormat_D24C8_UNORM = 0x18,
    DepthFormat_D32_FLOAT_S8X24_UINT = 0x19,
};
