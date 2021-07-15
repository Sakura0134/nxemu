#include "OpenGLResource.h"
#include "VideoNotification.h"

OpenGLTexture::OpenGLTexture() : 
    m_Handle(0)
{
}

OpenGLTexture::~OpenGLTexture()
{
    Release();
}

void OpenGLTexture::Create(GLenum target) 
{
    if (m_Handle != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glCreateTextures(target, 1, &m_Handle);
}

void OpenGLTexture::Release() 
{
    if (m_Handle == 0)
        return;

    glDeleteTextures(1, &m_Handle);
    m_Handle = 0;
}

void OpenGLTexture::TextureStorage3D(GLsizei Levels, GLenum InternalFormat, GLsizei Width, GLsizei Height, GLsizei Depth)
{
    if (m_Handle == 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glTextureStorage3D(m_Handle, Levels, InternalFormat, Width, Height, Depth);
}
