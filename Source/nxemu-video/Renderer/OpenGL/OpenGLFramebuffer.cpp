#include "OpenGLFramebuffer.h"
#include "OpenGLImageView.h"
#include "GpuTypes.h"
#include "VideoNotification.h"
#include <stdio.h>
#include <string.h>

OpenGLFramebuffer::OpenGLFramebuffer(OpenGLImageViewPtr * ColorBuffers, uint32_t NoOfColorBuffers, OpenGLImageView * DepthBuffer, uint8_t * DrawBuffers, uint32_t NoOfDrawBuffers, uint32_t Width, uint32_t Height) :
    m_Handle(0),
    m_BufferBits(GL_NONE),
    m_Ref(0)
{
    glGenFramebuffers(1, &m_Handle);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Handle);

    GLsizei NumBuffers = 0;
    GLenum GLDrawBuffers[NumRenderTargets];
    memset(&GLDrawBuffers, 0, sizeof(GLDrawBuffers));

    for (size_t i = 0; i < NoOfColorBuffers; i++)
    {
        OpenGLImageView * ColorBuffer = ColorBuffers[i].Get();
        if (ColorBuffer == nullptr) 
        {
            continue;
        }
        m_BufferBits |= GL_COLOR_BUFFER_BIT;
        if (i >= NoOfDrawBuffers) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            continue;
        }
        GLDrawBuffers[i] = GL_COLOR_ATTACHMENT0 + DrawBuffers[i];
        NumBuffers = (GLsizei)(i + 1);

        GLenum Attachment = (GLenum)(GL_COLOR_ATTACHMENT0 + i);
        AttachTexture(Attachment, ColorBuffer);
    }

    if (DepthBuffer != nullptr) 
    {
        m_BufferBits |= GL_DEPTH_BUFFER_BIT;
        if (SurfaceGetFormatType(DepthBuffer->Format()) == SurfaceType_DepthStencil)
        {
            m_BufferBits |= GL_STENCIL_BUFFER_BIT;
        }
        AttachTexture(AttachmentType(DepthBuffer->Format()), DepthBuffer);
    }

    if (NumBuffers > 1) 
    {
        glNamedFramebufferDrawBuffers(m_Handle, NumBuffers, GLDrawBuffers);
    } 
    else
    {
        glNamedFramebufferDrawBuffer(m_Handle, NumBuffers > 0 ? GLDrawBuffers[0] : GL_NONE);
    }
 
    glNamedFramebufferParameteri(m_Handle, GL_FRAMEBUFFER_DEFAULT_WIDTH, Width);
    glNamedFramebufferParameteri(m_Handle, GL_FRAMEBUFFER_DEFAULT_HEIGHT, Height);
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
    if (m_Handle != 0) 
    {
        glDeleteFramebuffers(1, &m_Handle);
        m_Handle = 0;
    }
}

void OpenGLFramebuffer::BindFramebuffer(GLenum Target) 
{
    if (m_Handle == 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    glBindFramebuffer(Target, m_Handle);
}

void OpenGLFramebuffer::AttachTexture(GLenum attachment, const OpenGLImageView* image_view)
{
    if (!image_view->IsFlagSet(ImageViewFlag_Slice)) 
    {
        glNamedFramebufferTexture(m_Handle, attachment, image_view->DefaultTexture()->m_Handle, 0);
        return;
    }
    const OpenGLTexturePtr & Texture = image_view->Texture(OpenGLImageViewType_e3D);
    if (image_view->Range().Extent().Layers() > 1) 
    {
        glNamedFramebufferTexture(m_Handle, attachment, Texture->m_Handle, 0);
    } 
    else 
    {
        GLint Layer = image_view->Range().Base().Layer();
        glNamedFramebufferTextureLayer(m_Handle, attachment, Texture->m_Handle, 0, Layer);
    }
}

GLenum OpenGLFramebuffer::AttachmentType(SurfacePixelFormat format) 
{
    switch (SurfaceGetFormatType(format)) 
    {
    case SurfaceType_Depth: return GL_DEPTH_ATTACHMENT;
    case SurfaceType_DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return GL_NONE;
}
