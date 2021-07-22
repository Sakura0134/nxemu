#include "OpenGLStagingBuffers.h"
#include "OpenGLResource.h"
#include "VideoNotification.h"

OpenGLStagingBuffers::OpenGLStagingBuffers(GLenum StorageFlags, GLenum MapFlags) : 
    m_StorageFlags(StorageFlags),
    m_MapFlags(MapFlags)
{
}

OpenGLStagingBuffers::~OpenGLStagingBuffers()
{
}

OpenGLStagingBuffer OpenGLStagingBuffers::RequestMap(uint32_t RequestedSize, bool InsertFence) 
{
    size_t Index = RequestBuffer(RequestedSize);
    OpenGLStagingBuffer & Buffer = m_Buffers[Index];
    OpenGLSyncPtr NullSyncPtr(nullptr);
    return OpenGLStagingBuffer(Buffer.Buffer(), Buffer.Map(), Buffer.Size(), InsertFence ? Buffer.Sync() : NullSyncPtr);
}

size_t OpenGLStagingBuffers::RequestBuffer(uint32_t RequestedSize) 
{
    size_t BufferIndex;
    if (FindBuffer(RequestedSize, BufferIndex))
    {
        return BufferIndex;
    }

    m_Buffers.emplace_back(RequestedSize, m_StorageFlags, m_MapFlags);
    return m_Buffers.size() - 1;
}

bool OpenGLStagingBuffers::FindBuffer(uint32_t /*RequestedSize*/, size_t & /*BufferIndex*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}
