#pragma once
#include "Renderer/Renderer.h"
#include "GpuTask.h"
#include "Framebuffer.h"

class SwapBufferTask :
    public CGpuTask 
{
public:
    SwapBufferTask(IRenderer& Renderer, CFramebuffer && Framebuffer);

    void Execute(void);

private:
    SwapBufferTask();
    SwapBufferTask(const SwapBufferTask&);
    SwapBufferTask& operator=(const SwapBufferTask&);

    CFramebuffer m_Framebuffer;
    IRenderer & m_Renderer;
};