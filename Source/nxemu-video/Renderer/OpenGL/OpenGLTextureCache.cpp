#include "OpenGLTextureCache.h"
#include "VideoNotification.h"

OpenGLTextureCache::OpenGLTextureCache(OpenGLRenderer & Renderer, CVideo & /*Video*/) :
    m_Renderer(Renderer)
{
}

bool OpenGLTextureCache::Init(void)
{
    return true;
}

void OpenGLTextureCache::UpdateRenderTargets(bool /*IsClear*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

void OpenGLTextureCache::WriteMemory(uint64_t /*CpuAddr*/, uint64_t /*Size*/)
{
}