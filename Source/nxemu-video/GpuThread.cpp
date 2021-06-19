#include "GpuThread.h"
#include "Renderer\OpenGL\RendererOpenGL.h"
#include "VideoNotification.h"
#include <Common/Util.h>

CGpuThread::CGpuThread(ISwitchSystem & SwitchSystem, CVideo & Video) : 
    CThread(stGpuThread),
    m_SwitchSystem(SwitchSystem),
    m_Video(Video),
    m_RenderInit(false),
    m_Running(true)
{
}

CGpuThread::~CGpuThread()
{
    if (isRunning())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
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

    while (m_Running)
    {
        m_TaskEvent.IsTriggered(SyncEvent::INFINITE_TIMEOUT);
        if (!m_Running)
        {
            break;
        }
       
        GpuTask Task;
        {
            CGuard Guard(m_cs);
            if (!m_Tasks.empty())
            {
                Task = std::move(m_Tasks.front());
                m_Tasks.pop_front();
            }
            else 
            {
                g_Notify->BreakPoint(__FILE__,__LINE__);
            }
            if (m_Tasks.empty()) 
            {
                m_TaskEvent.Reset();
            }
        }
        Task->Execute();
    }
}
