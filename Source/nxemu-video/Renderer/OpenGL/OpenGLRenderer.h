#pragma once
#include "OpenGLStateTracker.h"
#include "OpenGLWindow.h"
#include "OpenGLTextureCache.h"
#include "OpenGLStreamBuffer.h"
#include "OpenGLFenceManager.h"
#include "OpenGLDevice.h"
#include "Renderer\Renderer.h"
#include <map>
#include <stdint.h>

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
    void SyncDepthClamp();
    void SyncLogicOpState();
    void SyncCullMode();
    void SyncPrimitiveRestart();
    void SyncLineState();
    void SyncPolygonOffset();
    void SyncAlphaTest();
    void SyncBlendState();
    void SyncPointState();
    uint32_t CalculateVertexArraysSize() const;

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
    OpenGLDevice m_Device;
    OpenGLStreamBuffer m_StreamBuffer;
    bool m_QueuedCommands;
    TrackedPageMap m_TrackedPages;
    CriticalSection m_PageCS;
};
