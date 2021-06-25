#include "RendererOpenGL.h"
#include "VideoNotification.h"

RendererOpenGL::RendererOpenGL(ISwitchSystem & SwitchSystem, CVideo & Video) : 
    m_SwitchSystem(SwitchSystem), 
    m_Video(Video)
{
}

RendererOpenGL::~RendererOpenGL()
{
}

bool RendererOpenGL::Init()
{
    return true;
}

void RendererOpenGL::WaitForIdle(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

