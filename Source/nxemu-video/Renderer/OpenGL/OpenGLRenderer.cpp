#include "OpenGLRenderer.h"
#include "VideoNotification.h"

OpenGLRenderer::OpenGLRenderer(ISwitchSystem & SwitchSystem, CVideo & Video) : 
    m_SwitchSystem(SwitchSystem), 
    m_Video(Video)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}

bool OpenGLRenderer::Init()
{
    return true;
}

void OpenGLRenderer::WaitForIdle(void) 
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

