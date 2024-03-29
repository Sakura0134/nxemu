#pragma once
#include "Video.h"
#include "OpenGLItemPtr.h"
#include <stdint.h>
#include <queue>

class OpenGLRenderer;

class OpenGLFence;
typedef OpenGLItemPtr<OpenGLFence> OpenGLFencePtr;

class OpenGLFence
{
    friend OpenGLFencePtr;

public:
    OpenGLFence(uint32_t Payload, bool IsStubbed);
    OpenGLFence(uint64_t Address, uint32_t Payload, bool IsStubbed);
    ~OpenGLFence();

    void Queue();
    uint64_t GetAddress() const;
    uint32_t GetPayload() const;
    bool IsSemaphore() const;

private:
    OpenGLFence();
    OpenGLFence(const OpenGLFence&);
    OpenGLFence& operator=(const OpenGLFence&);

    uint64_t m_Address;
    uint32_t m_Payload;
    bool m_IsSemaphore;
    bool m_IsStubbed;
    uint32_t m_Ref;
};

class OpenGLFenceManager
{
public:
    OpenGLFenceManager(OpenGLRenderer & Renderer, CVideo & Video);

    void TickFrame();
    void SignalSemaphore(uint64_t Addr, uint32_t Value);
    void SignalSyncPoint(uint32_t Value);
    void WaitPendingFences();

private:
    OpenGLFenceManager();
    OpenGLFenceManager(const OpenGLFenceManager&);
    OpenGLFenceManager& operator=(const OpenGLFenceManager&);

    OpenGLFencePtr CreateFence(uint32_t Value, bool IsStubbed);
    OpenGLFencePtr CreateFence(uint64_t Addr, uint32_t Value, bool IsStubbed);
    void QueueFence(OpenGLFencePtr & Fence);
    bool IsFenceSignaled(OpenGLFencePtr & Fence) const;
    void ReleasePendingFences();
    bool ShouldWait() const;
    bool ShouldFlush() const;
    void PopAsyncFlushes();
    void CommitAsyncFlushes();
    void PopFence();

    OpenGLRenderer & m_Renderer;
    CVideo & m_Video;
    CVideoMemory & m_VideoMemory;
    std::queue<OpenGLFencePtr> m_Fences;
    std::vector<OpenGLFencePtr> m_DelayedDestoyFence[6];
    uint32_t m_DelayedDestoyIndex;
};
