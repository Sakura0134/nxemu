#include "OpenGLTextureCache.h"
#include "OpenGLStateTracker.h"
#include "Engine/Maxwell3D.h"
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

void OpenGLTextureCache::UpdateRenderTargets(bool /*IsClear*/)
{
    CStateTracker & StateTracker = m_Maxwell3D.StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_RenderTargets))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_RenderTargets);

    bool Force = StateTracker.Flag(OpenGLDirtyFlag_RenderTargetControl);
    StateTracker.FlagClear(OpenGLDirtyFlag_RenderTargetControl);

    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void OpenGLTextureCache::WriteMemory(uint64_t /*CpuAddr*/, uint64_t /*Size*/)
{
}