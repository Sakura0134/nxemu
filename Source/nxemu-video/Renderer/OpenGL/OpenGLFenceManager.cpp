#include "OpenGLFenceManager.h"
#include "VideoNotification.h"


OpenGLFence::OpenGLFence(uint32_t Payload, bool IsStubbed) :
    m_Payload(Payload),
    m_IsStubbed(IsStubbed),
    m_Ref(0)
{
}

OpenGLFence::~OpenGLFence()
{
}

void OpenGLFence::Queue()
{
    if (m_IsStubbed)
    {
        return;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

OpenGLFenceManager::OpenGLFenceManager(OpenGLRenderer & Renderer, CVideo & Video) :
    m_Renderer(Renderer),
    m_Video(Video)
{
}

OpenGLFencePtr OpenGLFenceManager::CreateFence(uint32_t value, bool IsStubbed)
{
    return OpenGLFencePtr(new OpenGLFence(value, IsStubbed));
}

void OpenGLFenceManager::QueueFence(OpenGLFencePtr & Fence)
{
    Fence->Queue();
}

void OpenGLFenceManager::SignalSyncPoint(uint32_t Value)
{
    ReleasePendingFences();
    bool Flush = ShouldFlush();
    CommitAsyncFlushes();
    OpenGLFencePtr Fence = CreateFence(Value, !Flush);
    m_Fences.push(Fence);
    QueueFence(Fence);
}

void OpenGLFenceManager::ReleasePendingFences()
{
    while (!m_Fences.empty())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

bool OpenGLFenceManager::ShouldFlush() const
{
    return false;
}

void OpenGLFenceManager::CommitAsyncFlushes()
{
}

