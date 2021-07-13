#include "OpenGLTextureCache.h"
#include "OpenGLStateTracker.h"
#include "Engine/Maxwell3D.h"
#include "Textures/Texture.h"
#include "OpenGLTypes.h"
#include "Video.h"
#include "VideoNotification.h"

OpenGLTextureCache::OpenGLTextureCache(OpenGLRenderer & Renderer, CVideo & Video) :
    m_Renderer(Renderer),
    m_Maxwell3D(Video.Maxwell3D())
{
}

bool OpenGLTextureCache::Init(void)
{
    return true;
}

void OpenGLTextureCache::UpdateRenderTargets(bool IsClear)
{
    CStateTracker & StateTracker = m_Maxwell3D.StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_RenderTargets))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_RenderTargets);

    bool Force = StateTracker.Flag(OpenGLDirtyFlag_RenderTargetControl);
    StateTracker.FlagClear(OpenGLDirtyFlag_RenderTargetControl);

    for (uint8_t i = 0, n = sizeof(m_ColorBuffer) / sizeof(m_ColorBuffer[0]); i < n; i++)
    {
        OpenGLImageViewPtr & ColorBuffer = m_ColorBuffer[i];
        if (StateTracker.Flag(OpenGLDirtyFlag_ColorBuffer0 + i) || Force)
        {
            StateTracker.FlagClear(OpenGLDirtyFlag_ColorBuffer0 + i);
            OpenGLImageViewPtr NewColorBuffer = FindColorBuffer(i, IsClear);
            if (ColorBuffer != NewColorBuffer)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
        if (ColorBuffer.Get() != nullptr)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

OpenGLImage * OpenGLTextureCache::GetImage(const OpenGLImage & /*Info*/, uint64_t /*GpuAddr*/, uint32_t /*Options*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

OpenGLImageViewPtr OpenGLTextureCache::FindColorBuffer(size_t Index, bool IsClear)
{
    const CMaxwell3D::Registers & Regs = m_Maxwell3D.Regs();
    if (Index >= Regs.RTControl.Count)
    {
        return nullptr;
    }
    const CMaxwell3D::tyRenderTarget & RenderTarget = Regs.RenderTarget[Index];
    if (RenderTarget.Address() == 0 || RenderTarget.Format == RenderTargetFormat_NONE)
    {
        return nullptr;
    }
    uint64_t GPUAddr = RenderTarget.Address();
    OpenGLImage * Image = GetImage(OpenGLImage(Regs.RenderTarget[Index], MsaaModeSamples(Regs.MultisampleMode)), GPUAddr, IsClear ? OpenGLOptions_Samples : 0);
    if (Image == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

void OpenGLTextureCache::WriteMemory(uint64_t /*CpuAddr*/, uint64_t /*Size*/)
{
}