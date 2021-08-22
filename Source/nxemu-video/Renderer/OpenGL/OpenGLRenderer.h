#pragma once
#include "OpenGLStateTracker.h"
#include "OpenGLWindow.h"
#include "OpenGLTextureCache.h"
#include "OpenGLFenceManager.h"
#include "Renderer\Renderer.h"
#include <map>

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
    void Clear();
    void Draw(bool IsIndexed, bool IsInstanced);

    void TrackRasterizerMemory(uint64_t CpuAddr, uint64_t Size, bool Track);

private:
    OpenGLRenderer();
    OpenGLRenderer(const OpenGLRenderer&);
    OpenGLRenderer& operator=(const OpenGLRenderer&);

    void SyncFragmentColorClampState();
    void SyncFramebufferSRGB();
    void SyncRasterizeEnable();
    void SyncStencilTestState();
    void SyncScissorTest();
    void SyncViewport();
    void SyncPolygonModes();
    void SyncColorMask();
    void SyncMultiSampleState();
    void SyncDepthTestState();

    static void OpenGLEnable(GLenum Cap, bool Enable);

    typedef struct 
    {
        uint64_t Start;
        uint64_t Size;
    } TrackedPage;

    typedef std::map<uint64_t, TrackedPage> TrackedPageMap;
    
    OpenGLStateTracker m_StateTracker;
    OpenGLTextureCache m_TextureCache;
    OpenGLFenceManager m_FenceManager;
    OpenGLWindow m_OpenGLWindow;
    EmulatorWindow & m_EmulatorWindow;
    ISwitchSystem & m_SwitchSystem;
    CVideo & m_Video;
    bool m_QueuedCommands;
    TrackedPageMap m_TrackedPages;
    CriticalSection m_PageCS;
};
