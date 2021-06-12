#include <nxemu-core\hle\Display\Nvidia\NvDisp0.h>
#include <nxemu-core\hle\Display\Nvidia\NvDriver.h>

CNvDisp0::CNvDisp0(CNvDriver& NvDriver) :
    m_NvDriver(NvDriver)
{
}

CNvDisp0::~CNvDisp0()
{
}

void CNvDisp0::flip(uint32_t BufferHandle, uint32_t Offset, uint32_t Format, uint32_t Width, uint32_t Height, uint32_t Stride, BufferTransformFlags Transform, const CBufferQueue::Rectangle& CropRect)
{
    NvMapHandle & Map = m_NvDriver.NvMap().FindNvMapHandle(BufferHandle);

    m_NvDriver.Video().SwapBuffers(Map.Address(), Offset, Format, Width, Height, Stride, Transform, CropRect.Left, CropRect.Top, CropRect.Right, CropRect.Bottom);
}