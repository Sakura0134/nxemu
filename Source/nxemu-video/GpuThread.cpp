#include "GpuThread.h"
#include "Renderer\OpenGL\RendererOpenGL.h"
#include "VideoNotification.h"
#include <Common/Util.h>

CGpuThread::CGpuThread(ISwitchSystem & SwitchSystem, CVideo & Video) : 
    CThread(stGpuThread),
    m_SwitchSystem(SwitchSystem),
    m_Video(Video),
    m_RenderInit(false)
{
}

CGpuThread::~CGpuThread()
{
}

bool CGpuThread::StartThread()
{
    if (isRunning())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    m_RenderInit = false;
    Start(this);
    for (uint32_t i = 0 , n = 10000; i < n; i++) 
    {
        if (m_RenderInit)
        {
            break;
        }
        nxutil::Sleep(10);
    }
    return m_RenderInit;
}

void CGpuThread::GpuThread(void)
{
    m_Renderer.reset(new RendererOpenGL(m_SwitchSystem, m_Video));
    if (!m_Renderer->Init()) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_RenderInit = true;

    g_Notify->BreakPoint(__FILE__, __LINE__);
}
