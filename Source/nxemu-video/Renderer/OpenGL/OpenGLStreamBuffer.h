#pragma once
#include "OpenGLResource.h"
#include <stdint.h>

__interface ISwitchSystem;
class OpenGLDevice;
class OpenGLStateTracker;
class CVideoMemory;
class CVideo;

class OpenGLStreamBuffer
{
public:
    enum 
    {
        BUFFER_SIZE = 256 * 1024 * 1024
    };

    OpenGLStreamBuffer(ISwitchSystem & System, CVideo & Video, OpenGLStateTracker & StateTracker);
    ~OpenGLStreamBuffer();

    bool Init(const OpenGLDevice & Device);
    void Map(uint32_t MaxSize);
    uint64_t UploadMemory(uint64_t GpuAddr, uint32_t Size, uint32_t Alignment);
    uint64_t UploadHostMemory(const void * Data, uint32_t Size, uint32_t Alignment);

    OpenGLBufferPtr & Buffer() { return m_Buffer; }

private:
    OpenGLStreamBuffer(void);
    OpenGLStreamBuffer(const OpenGLStreamBuffer&);
    OpenGLStreamBuffer& operator=(const OpenGLStreamBuffer&);

    void AlignBuffer(uint32_t Alignment);

    ISwitchSystem & m_System;
    CVideoMemory & m_VideoMemory;
    OpenGLStateTracker & m_StateTracker;
    OpenGLBufferPtr m_Buffer;
    GLintptr m_BufferPos;
    GLintptr m_BufferPosBase;
    GLsizeiptr m_MappedSize;
    uint8_t * m_MappedPtr;
    uint8_t * m_BufferPtr;
    uint32_t m_BufferOffset;
};
