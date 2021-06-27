#include "OpenGLRenderer.h"
#include "EmulatorWindow.h"
#include "VideoNotification.h"
#include "Video.h"
#include <glad/glad.h>

#pragma comment(lib, "opengl32.lib")

OpenGLRenderer::OpenGLRenderer(ISwitchSystem & SwitchSystem, CVideo & Video) : 
    m_EmulatorWindow(Video.Window()),
    m_SwitchSystem(SwitchSystem), 
    m_Video(Video)
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

void OpenGLRenderer::WaitForIdle(void) 
{
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_ELEMENT_ARRAY_BARRIER_BIT |
                    GL_UNIFORM_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT |
                    GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_COMMAND_BARRIER_BIT |
                    GL_PIXEL_BUFFER_BARRIER_BIT | GL_TEXTURE_UPDATE_BARRIER_BIT |
                    GL_BUFFER_UPDATE_BARRIER_BIT | GL_TRANSFORM_FEEDBACK_BARRIER_BIT |
                    GL_SHADER_STORAGE_BARRIER_BIT | GL_QUERY_BUFFER_BARRIER_BIT);
}

