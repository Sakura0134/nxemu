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
    ~OpenGLFence();

    void Queue();

private:
    OpenGLFence();
    OpenGLFence(const OpenGLFence&);
    OpenGLFence& operator=(const OpenGLFence&);

    uint32_t m_Payload;
    bool m_IsStubbed;
    uint32_t m_Ref;
};

class OpenGLFenceManager
{
public:
    OpenGLFenceManager(OpenGLRenderer & Renderer, CVideo & Video);

    void SignalSyncPoint(uint32_t Value);

private:
    OpenGLFenceManager();
    OpenGLFenceManager(const OpenGLFenceManager&);
    OpenGLFenceManager& operator=(const OpenGLFenceManager&);

    OpenGLFencePtr CreateFence(uint32_t Value, bool IsStubbed);
    void QueueFence(OpenGLFencePtr & Fence);
    void ReleasePendingFences();
    bool ShouldFlush() const;
    void CommitAsyncFlushes();

    OpenGLRenderer & m_Renderer;
    CVideo & m_Video;
    std::queue<OpenGLFencePtr> m_Fences;
};
