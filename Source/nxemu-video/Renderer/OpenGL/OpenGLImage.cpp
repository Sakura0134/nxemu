#include "OpenGLImage.h"
#include "VideoNotification.h"

OpenGLImage::OpenGLImage(const CMaxwell3D::tyRenderTarget & RenderTarget, uint32_t Samples) :
    m_Ref(0)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}

OpenGLImage::~OpenGLImage()
{
}
