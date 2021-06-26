#pragma once
#include "Renderer\Renderer.h"

__interface ISwitchSystem;
class CVideo;

class OpenGLRenderer : 
    public IRenderer
{
public:
    OpenGLRenderer(ISwitchSystem& SwitchSystem, CVideo& Video);
    ~OpenGLRenderer();

    //IRenderer
    bool Init();
    void WaitForIdle(void);

private:
    OpenGLRenderer();
    OpenGLRenderer(const OpenGLRenderer&);
    OpenGLRenderer& operator=(const OpenGLRenderer&);

    ISwitchSystem & m_SwitchSystem;
    CVideo & m_Video;
};
