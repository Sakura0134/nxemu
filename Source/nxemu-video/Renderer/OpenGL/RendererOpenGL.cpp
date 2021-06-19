#include "RendererOpenGL.h"

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

