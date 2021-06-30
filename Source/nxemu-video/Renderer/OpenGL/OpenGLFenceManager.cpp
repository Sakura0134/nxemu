#include "OpenGLFenceManager.h"
#include "VideoNotification.h"


OpenGLFence::OpenGLFence(uint32_t Payload, bool IsStubbed) :
    m_Address(0),
    m_Payload(Payload),
    m_IsSemaphore(false),
    m_IsStubbed(IsStubbed),
    m_Ref(0)
{
}

OpenGLFence::OpenGLFence(uint64_t Address, uint32_t Payload, bool IsStubbed) :
    m_Address(Address),
    m_Payload(Payload),
    m_IsSemaphore(true),
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

uint64_t OpenGLFence::GetAddress() const
{
    return m_Address;
}

uint32_t OpenGLFence::GetPayload() const
{
    return m_Payload;
}

bool OpenGLFence::IsSemaphore() const
{
    return m_IsSemaphore;
}

OpenGLFenceManager::OpenGLFenceManager(OpenGLRenderer & Renderer, CVideo & Video) :
    m_Renderer(Renderer),
    m_Video(Video),
    m_VideoMemory(Video.VideoMemory()),
    m_DelayedDestoyIndex(0)
{
}

OpenGLFencePtr OpenGLFenceManager::CreateFence(uint32_t value, bool IsStubbed)
{
    return OpenGLFencePtr(new OpenGLFence(value, IsStubbed));
}

OpenGLFencePtr OpenGLFenceManager::CreateFence(uint64_t Addr, uint32_t Value, bool IsStubbed)
{
    return OpenGLFencePtr(new OpenGLFence(Addr, Value, IsStubbed));
}

void OpenGLFenceManager::QueueFence(OpenGLFencePtr & Fence)
{
    Fence->Queue();
}

bool OpenGLFenceManager::IsFenceSignaled(OpenGLFencePtr & /*Fence*/) const
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

void OpenGLFenceManager::SignalSemaphore(uint64_t Addr, uint32_t Value)
{
    ReleasePendingFences();
    bool Flush = ShouldFlush();
    CommitAsyncFlushes();
    OpenGLFencePtr Fence = CreateFence(Addr, Value, !Flush);
    m_Fences.push(Fence);
    QueueFence(Fence);
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

void OpenGLFenceManager::WaitPendingFences()
{
    while (!m_Fences.empty())
    {
        OpenGLFencePtr & Fence = m_Fences.front();
        if (ShouldWait())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            //WaitFence(Fence);
        }
        PopAsyncFlushes();
        if (Fence->IsSemaphore())
        {
            uint32_t Payload = Fence->GetPayload();
            m_VideoMemory.WriteBuffer(Fence->GetAddress(), &Payload, sizeof(Payload), false);
        }
        else
        {
            m_Video.IncrementSyncPoint(Fence->GetPayload());
        }
        PopFence();
    }
}

void OpenGLFenceManager::ReleasePendingFences()
{
    while (!m_Fences.empty())
    {
        OpenGLFencePtr & Fence = m_Fences.front();
        if (ShouldWait() && !IsFenceSignaled(Fence))
        {
            return;
        }
        PopAsyncFlushes();
        if (Fence->IsSemaphore())
        {
            uint32_t Payload = Fence->GetPayload();
            m_VideoMemory.WriteBuffer(Fence->GetAddress(), &Payload, sizeof(Payload), false);
        }
        else
        {
            m_Video.IncrementSyncPoint(Fence->GetPayload());
        }
        PopFence();
    }
}

bool OpenGLFenceManager::ShouldWait() const
{
    return false;
}

bool OpenGLFenceManager::ShouldFlush() const
{
    return false;
}

void OpenGLFenceManager::PopAsyncFlushes()
{
}

void OpenGLFenceManager::CommitAsyncFlushes()
{
}

void OpenGLFenceManager::PopFence()
{
    m_DelayedDestoyFence[m_DelayedDestoyIndex].push_back(std::move(m_Fences.front()));
    m_Fences.pop();
}
