#pragma once
#include "Engine\Maxwell3D.h"

class OpenGLImage
{
public:
    OpenGLImage();
    OpenGLImage(const OpenGLImage & Image);
    OpenGLImage(const CMaxwell3D::tyRenderTarget& RenderTarget, uint32_t Samples);
    ~OpenGLImage();

private:
    OpenGLImage& operator=(const OpenGLImage&);

    uint32_t m_Ref;
};
