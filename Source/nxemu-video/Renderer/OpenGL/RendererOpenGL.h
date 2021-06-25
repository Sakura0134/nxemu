#pragma once
#include "Renderer\Renderer.h"

__interface ISwitchSystem;
class CVideo;

class RendererOpenGL : 
    public IRenderer
{
public:
    RendererOpenGL(ISwitchSystem& SwitchSystem, CVideo& Video);
    ~RendererOpenGL();

    //IRenderer
    bool Init();
    void WaitForIdle(void);

private:
    RendererOpenGL();
    RendererOpenGL(const RendererOpenGL&);
    RendererOpenGL& operator=(const RendererOpenGL&);

    ISwitchSystem & m_SwitchSystem;
    CVideo & m_Video;
};
