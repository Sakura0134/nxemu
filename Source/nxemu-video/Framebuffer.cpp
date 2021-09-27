#include "Framebuffer.h"

CFramebuffer::CFramebuffer(uint64_t Address, uint32_t Offset, GPUPixelFormat Format, uint32_t Width, uint32_t Height, uint32_t Stride, ::TransformFlags Transform, int32_t CropLeft, int32_t CropTop, int32_t CropRight, int32_t CropBottom) :
    m_Address(Address),
    m_Offset(Offset),
    m_Width(Width),
    m_Height(Height),
    m_Stride(Stride),
    m_PixelFormat(Format),
    m_TransformFlags(Transform),
    m_CropRect(CropLeft, CropTop, CropRight, CropBottom)
{
}