#include "GpuThread.h"
#include "Renderer\OpenGL\OpenGLRenderer.h"
#include "Video.h"
#include "VideoNotification.h"
#include "Task/CommandList.h"
#include "Task/CommandListEnd.h"
#include "Task/SwapBuffer.h"
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

void CGpuThread::PushCommands(const uint64_t* Entries, uint32_t NoOfEntries)
{
    PushCommand(std::move(std::make_shared<CommandListTask>(m_Video, *m_Renderer, Entries, NoOfEntries)));
}

void CGpuThread::SwapBuffers(CFramebuffer && Framebuffer)
{
    PushCommand(std::move(std::make_shared<SwapBufferTask>(*m_Renderer, std::move(Framebuffer))));
}

void CGpuThread::OnCommandListEnd()
{
    PushCommand(std::move(std::make_shared<CommandListEndTask>(*m_Renderer)));
}

void CGpuThread::PushCommand(GpuTask && Task)
{
    CGuard Guard(m_cs);
    m_Tasks.push_back(std::move(Task));
    m_TaskEvent.Trigger();
}

void CGpuThread::GpuThread(void)
{
    m_Renderer.reset(new OpenGLRenderer(m_SwitchSystem, m_Video));
    if (!m_Renderer->Init())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_Video.BindRenderer(m_Renderer.get());
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
    m_Video.BindRenderer(nullptr);
}
