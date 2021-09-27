#pragma once

class OpenGLWindow
{
public:
    OpenGLWindow(void);

    bool Initialize(void * RenderSurface);
    void SwapBuffers(void);

private:
    OpenGLWindow(const OpenGLWindow &);
    OpenGLWindow & operator=(const OpenGLWindow &);

    void * m_hGLRC;
};