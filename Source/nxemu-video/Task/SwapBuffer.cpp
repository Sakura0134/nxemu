#include "SwapBuffer.h"
#include "Video.h"
#include "VideoNotification.h"

SwapBufferTask::SwapBufferTask(IRenderer & Renderer, CFramebuffer && Framebuffer) :
    m_Framebuffer(std::move(Framebuffer)),
    m_Renderer(Renderer)
{
}

void SwapBufferTask::Execute(void)
{
    m_Renderer.SwapBuffers(m_Framebuffer);
}