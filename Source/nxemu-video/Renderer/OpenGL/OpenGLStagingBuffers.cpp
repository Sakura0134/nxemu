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

bool OpenGLStagingBuffers::FindBuffer(uint32_t RequestedSize, size_t & BufferIndex)
{
    uint32_t SmallestBuffer = 0xFFFFFFFF;
    bool Found = false;
    for (size_t i = 0, n = m_Buffers.size(); i < n; i++)
    {
        uint32_t BufferSize = m_Buffers[i].Size();
        if (BufferSize < RequestedSize || BufferSize >= SmallestBuffer)
        {
            continue;
        }
        OpenGLSyncPtr & Sync = m_Buffers[i].Sync();
        if (Sync->Created()) 
        {
            GLint Status;
            Sync->GetSynciv(GL_SYNC_STATUS, 1, nullptr, &Status);
            if (Status != GL_SIGNALED) 
            {
                continue;
            }
            Sync->Release();
        }
        SmallestBuffer = BufferSize;
        BufferIndex = i;
        Found = true;
    }
    return Found;
}
