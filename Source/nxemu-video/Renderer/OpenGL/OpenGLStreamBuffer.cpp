#include "OpenGLStreamBuffer.h"
#include "OpenGLDevice.h"
#include "Video.h"
#include "VideoNotification.h"
#include <Common\Align.h>

OpenGLStreamBuffer::OpenGLStreamBuffer(ISwitchSystem & System, CVideo & Video, OpenGLStateTracker & StateTracker) :
    m_System(System),
    m_VideoMemory(Video.VideoMemory()),
    m_StateTracker(StateTracker),
    m_Buffer(new OpenGLBuffer), 
    m_BufferPos(0),
    m_BufferPosBase(0),
    m_MappedSize(0), 
    m_MappedPtr(nullptr), 
    m_BufferPtr(nullptr),
    m_BufferOffset(0)
{
}

OpenGLStreamBuffer::~OpenGLStreamBuffer() 
{
}

bool OpenGLStreamBuffer::Init(const OpenGLDevice & Device) 
{
    m_Buffer->Create();

    GLbitfield Flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT;
    m_Buffer->NamedBufferStorage(BUFFER_SIZE, nullptr, Flags);
    m_MappedPtr = (uint8_t*)m_Buffer->MapNamedBufferRange(0, BUFFER_SIZE, Flags | GL_MAP_FLUSH_EXPLICIT_BIT);

    if (Device.UseAssemblyShaders() || Device.HasVertexBufferUnifiedMemory()) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return true;
}

void OpenGLStreamBuffer::Map(uint32_t Size) 
{
    if (Size > BUFFER_SIZE || m_BufferPos + Size > BUFFER_SIZE)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_MappedSize = Size;
    m_BufferPos = AlignUp(m_BufferPos, 4);
    m_BufferPosBase = m_BufferPos;
    m_BufferPtr = m_MappedPtr + m_BufferPos;
    m_BufferOffset = (uint32_t)m_BufferPos;
}