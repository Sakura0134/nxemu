#pragma once
#include "OpenGLWindow.h"
#include "OpenGLFenceManager.h"
#include "Renderer\Renderer.h"

__interface ISwitchSystem;
class CVideo;
class EmulatorWindow;

class OpenGLRenderer : 
    public IRenderer
{
public:
    OpenGLRenderer(ISwitchSystem & SwitchSystem, CVideo & Video);
    ~OpenGLRenderer();

    //IRenderer
    bool Init();
    void InvalidateRegion(uint64_t Addr, uint64_t Size);
    void FlushCommands(void);
    void WaitForIdle(void);
    void SignalSyncPoint(uint32_t Value);
    void SignalSemaphore(uint64_t Addr, uint32_t Value);
    void ReleaseFences(void);

private:
    OpenGLRenderer();
    OpenGLRenderer(const OpenGLRenderer&);
    OpenGLRenderer& operator=(const OpenGLRenderer&);

    OpenGLFenceManager m_FenceManager;
    OpenGLWindow m_OpenGLWindow;
    EmulatorWindow & m_EmulatorWindow;
    ISwitchSystem & m_SwitchSystem;
    CVideo & m_Video;
};
