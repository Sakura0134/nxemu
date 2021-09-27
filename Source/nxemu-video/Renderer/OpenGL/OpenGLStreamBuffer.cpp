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

uint64_t OpenGLStreamBuffer::UploadMemory(uint64_t GpuAddr, uint32_t Size, uint32_t Alignment)
{
    uint64_t CpuAddr;
    if (!m_VideoMemory.GpuToCpuAddress(GpuAddr, CpuAddr))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    bool IsGranular = m_VideoMemory.IsGranularRange(GpuAddr, Size);
    AlignBuffer(Alignment);
    uint64_t UploadedOffset = m_BufferOffset;
    if (IsGranular) 
    {
        std::memcpy(m_BufferPtr, m_VideoMemory.GetPointer(GpuAddr), Size);
    } 
    else 
    {
        m_VideoMemory.ReadBuffer(GpuAddr, m_BufferPtr, Size);
    }
    m_BufferPtr += Size;
    m_BufferOffset += Size;
    return UploadedOffset;
}

uint64_t OpenGLStreamBuffer::UploadHostMemory(const void * Data, uint32_t Size, uint32_t Alignment) 
{
    AlignBuffer(Alignment);
    uint64_t UploadedOffset = m_BufferOffset;
    std::memcpy(m_BufferPtr, Data, Size);

    m_BufferPtr += Size;
    m_BufferOffset += Size;
    return UploadedOffset;
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

void OpenGLStreamBuffer::Unmap()
{
    GLsizeiptr Size = m_BufferOffset - m_BufferPosBase;
    if (Size > m_MappedSize)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    if (Size > 0)
    {
        m_Buffer->FlushMappedNamedBufferRange(m_BufferPos, Size);
    }
    m_BufferPos += Size;
}

void OpenGLStreamBuffer::AlignBuffer(uint32_t Alignment)
{
    uint32_t OffsetAligned = AlignUp(m_BufferOffset, Alignment);
    m_BufferPtr += OffsetAligned - m_BufferOffset;
    m_BufferOffset = OffsetAligned;
}
