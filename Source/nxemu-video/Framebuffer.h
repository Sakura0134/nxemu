#pragma once
#include <stdint.h>
#include "GpuTypes.h"
#include "util/Rectangle.h"

class CFramebuffer
{
public:
    CFramebuffer(uint64_t Address, uint32_t Offset, GPUPixelFormat Format, uint32_t Width, uint32_t Height, uint32_t Stride, TransformFlags Transform, int32_t CropLeft, int32_t CropTop, int32_t CropRight, int32_t CropBottom);
  
    uint64_t Address() const { return m_Address; }
    uint32_t Offset() const { return m_Offset; }
    uint32_t Width() const { return m_Width; }
    uint32_t Height() const { return m_Height; }
    uint32_t Stride() const { return m_Stride; }
    GPUPixelFormat PixelFormat() const { return m_PixelFormat; }
    TransformFlags Flags() const { return m_TransformFlags; }
    const CRectangle<int> & CropRect() const { return m_CropRect; }

private:
    uint64_t m_Address;
    uint32_t m_Offset;
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_Stride;
    GPUPixelFormat m_PixelFormat;
    TransformFlags m_TransformFlags;
    CRectangle<int> m_CropRect;
};