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
private:
    OpenGLStreamBuffer(void);
    OpenGLStreamBuffer(const OpenGLStreamBuffer&);
    OpenGLStreamBuffer& operator=(const OpenGLStreamBuffer&);

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
