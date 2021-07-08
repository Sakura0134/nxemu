#include "OpenGLRenderer.h"
#include "EmulatorWindow.h"
#include "VideoNotification.h"
#include "Video.h"
#include <glad/glad.h>

#pragma comment(lib, "opengl32.lib")

OpenGLRenderer::OpenGLRenderer(ISwitchSystem & SwitchSystem, CVideo & Video) : 
    m_EmulatorWindow(Video.Window()),
    m_SwitchSystem(SwitchSystem), 
    m_Video(Video),
    m_StateTracker(Video),
    m_FenceManager(*this, Video)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}

bool OpenGLRenderer::Init()
{
    if (!m_OpenGLWindow.Initialize(m_EmulatorWindow.RenderSurface())) 
    {
        return false;
    }
    if (!GLAD_GL_VERSION_4_3) 
    {
        return false;
    }
    glClearColor(0.0f, 0.0f, 0.0, 0.0f);
    return true;
}

void OpenGLRenderer::InvalidateRegion(uint64_t Addr, uint64_t Size)
{
    if (Addr == 0 || Size == 0) 
    {
        return;
    }
}

void OpenGLRenderer::FlushCommands(void)
{
}

void OpenGLRenderer::WaitForIdle(void) 
{
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_ELEMENT_ARRAY_BARRIER_BIT |
                    GL_UNIFORM_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT |
                    GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_COMMAND_BARRIER_BIT |
                    GL_PIXEL_BUFFER_BARRIER_BIT | GL_TEXTURE_UPDATE_BARRIER_BIT |
                    GL_BUFFER_UPDATE_BARRIER_BIT | GL_TRANSFORM_FEEDBACK_BARRIER_BIT |
                    GL_SHADER_STORAGE_BARRIER_BIT | GL_QUERY_BUFFER_BARRIER_BIT);
}

void OpenGLRenderer::SignalSyncPoint(uint32_t Value)
{
    m_FenceManager.SignalSyncPoint(Value);
}

void OpenGLRenderer::SignalSemaphore(uint64_t Addr, uint32_t Value) 
{
    m_FenceManager.SignalSemaphore(Addr, Value);
}

void OpenGLRenderer::ReleaseFences(void) 
{
    m_FenceManager.WaitPendingFences();
}

void OpenGLRenderer::Clear() 
{
    const CMaxwell3D::Registers & Regs = m_Video.Maxwell3D().Regs();
    bool UseColor = false, UseDepth = false, UseStencil = false;
    if (Regs.ClearBuffers.R != 0 || Regs.ClearBuffers.G != 0 || Regs.ClearBuffers.B != 0 || Regs.ClearBuffers.A != 0)
    {
        UseColor = true;

        GLuint Index = Regs.ClearBuffers.RT;
        m_StateTracker.NotifyColorMask((uint8_t)Index);
        glColorMaski(Index, Regs.ClearBuffers.R != 0, Regs.ClearBuffers.G != 0, Regs.ClearBuffers.B != 0, Regs.ClearBuffers.A != 0);

        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    if (Regs.ClearBuffers.Z != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    if (Regs.ClearBuffers.S != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    if (!UseColor && !UseDepth && !UseStencil)
    {
        return;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

