#pragma once
#include <nxemu-core\hle\Display\Nvidia\BufferQueue.h>
#include <stdint.h>

class CNvDriver;

class CNvDisp0
{
public:
    CNvDisp0(CNvDriver& NvDriver);
    ~CNvDisp0();

    void flip(uint32_t BufferHandle, uint32_t Offset, uint32_t Format, uint32_t Width, uint32_t Height, uint32_t Stride, BufferTransformFlags Transform, const CBufferQueue::Rectangle& CropRect);

private:
    CNvDisp0();
    CNvDisp0(const CNvDisp0&);
    CNvDisp0& operator=(const CNvDisp0&);

    CNvDriver& m_NvDriver;
};