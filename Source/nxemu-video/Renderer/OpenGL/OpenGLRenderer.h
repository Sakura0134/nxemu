#pragma once
#include "OpenGLStateTracker.h"
#include "OpenGLScreen.h"
#include "OpenGLWindow.h"
#include "OpenGLShaderCache.h"
#include "OpenGLTextureCache.h"
#include "OpenGLStreamBuffer.h"
#include "OpenGLFenceManager.h"
#include "OpenGLDevice.h"
#include "OpenGLProgramManager.h"
#include "Renderer\Renderer.h"
#include <vector>
#include <map>
#include <stdint.h>

__interface ISwitchSystem;
class CVideo;
class EmulatorWindow;

class OpenGLRenderer : 
    public IRenderer
{
    enum 
    {
        MAX_TEXTURES = 192,
        MAX_IMAGES = 48,
        MAX_IMAGE_VIEWS = MAX_TEXTURES + MAX_IMAGES,
        NUM_SUPPORTED_VERTEX_ATTRIBUTES = 16,
        NUM_SUPPORTED_VERTEX_BINDINGS = 16,
    };

    typedef struct
    {
        uint64_t Start;
        uint64_t Size;
    } TrackedPage;

    typedef std::map<uint64_t, TrackedPage> TrackedPageMap;

public:
    OpenGLRenderer(ISwitchSystem & SwitchSystem, CVideo & Video);
    ~OpenGLRenderer();

    //IRenderer
    bool Init();
    void SwapBuffers(const CFramebuffer & Framebuffer);
    void InvalidateRegion(uint64_t CpuAddr, uint32_t Size);
    void FlushCommands(void);
    void SyncGuestHost(void);
    void WaitForIdle(void);
    void SignalSyncPoint(uint32_t Value);
    void SignalSemaphore(uint64_t Addr, uint32_t Value);
    void ReleaseFences(void);
    void Clear();
    void Draw(bool IsIndexed, bool IsInstanced);
    bool IsTextureHandlerSizeKnown() const;

    void TickFrame();
    void TrackRasterizerMemory(uint64_t CpuAddr, uint64_t Size, bool Track);

private:
    OpenGLRenderer();
    OpenGLRenderer(const OpenGLRenderer&);
    OpenGLRenderer& operator=(const OpenGLRenderer&);

    void BindTextures(const OpenGLCompiledShaderPtr * Shaders, uint32_t NoOfShaders);
    void BeginTransformFeedback(GLenum primitive_mode);
    void EndTransformFeedback();
    void SetupVertexFormat();
    void SetupVertexBuffer();
    void SetupVertexInstances();
    GLintptr SetupIndexBuffer();
    void SetupShaders();
    void SetupDrawConstBuffers(uint32_t StageIndex, OpenGLCompiledShaderPtr & Shader);
    void SetupDrawTextures(uint32_t StageIndex, OpenGLCompiledShaderPtr & Shader);
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
    void SyncClipEnabled(uint32_t ClipMask);
    uint32_t CalculateVertexArraysSize() const;

    static void OpenGLEnable(GLenum Cap, bool Enable);
    static OpenGLImageViewType ImageViewTypeFromEntry(const ShaderSamplerEntry& Entry);

    OpenGLStateTracker m_StateTracker;
    OpenGLTextureCache m_TextureCache;
    OpenGLShaderCache m_ShaderCache;
    OpenGLFenceManager m_FenceManager;
    OpenGLWindow m_OpenGLWindow;
    EmulatorWindow & m_EmulatorWindow;
    ISwitchSystem & m_SwitchSystem;
    CVideo & m_Video;
    OpenGLDevice m_Device;
    OpenGLScreen m_Screen;
    OpenGLProgramManager m_ProgramManager;
    OpenGLStreamBuffer m_StreamBuffer;
    std::vector<uint32_t> m_ImageViewIndices;
    OpenGLImageViewPtr m_ImageViews[MAX_IMAGE_VIEWS];
    std::vector<OpenGLSamplerPtr> m_Samplers;
    OpenGLTexturePtr m_Textures[MAX_TEXTURES];
    bool m_QueuedCommands;
    uint32_t m_LastClipDistanceMask;
    bool m_IsTextureHandlerSizeKnown;
    TrackedPageMap m_TrackedPages;
    CriticalSection m_PageCS;
};
