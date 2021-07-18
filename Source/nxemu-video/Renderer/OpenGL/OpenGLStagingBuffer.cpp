#include "OpenGLStagingBuffer.h"

OpenGLStagingBuffer::OpenGLStagingBuffer(OpenGLBufferPtr & Buffer, uint8_t * Map, uint32_t Size, OpenGLSyncPtr & Sync) :
    m_Buffer(Buffer), 
    m_Sync(Sync),
    m_Map(Map), 
    m_Size(Size) 
{
}

OpenGLStagingBuffer::OpenGLStagingBuffer(uint32_t Size, GLenum StorageFlags, GLenum MapFlags) :
    m_Buffer(new OpenGLBuffer), 
    m_Sync(new OpenGLSync),
    m_Map(nullptr)
{
    m_Buffer->Create();
    m_Buffer->NamedBufferStorage(Size, nullptr, StorageFlags | GL_MAP_PERSISTENT_BIT); 
    m_Map = (uint8_t*)(m_Buffer->MapNamedBufferRange(0, Size, MapFlags | GL_MAP_PERSISTENT_BIT));
    m_Size = Size;
}

OpenGLStagingBuffer::OpenGLStagingBuffer(const OpenGLStagingBuffer& rhs) :
    m_Sync(rhs.m_Sync),
    m_Buffer(rhs.m_Buffer), 
    m_Map(rhs.m_Map), 
    m_Size(rhs.m_Size)
{
}

OpenGLStagingBuffer::~OpenGLStagingBuffer() 
{
}