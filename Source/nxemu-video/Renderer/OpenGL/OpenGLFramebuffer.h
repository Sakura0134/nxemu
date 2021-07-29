#pragma once
#include "OpenGLItemPtr.h"
#include "surface.h"
#include <glad/glad.h>
#include <stdint.h>

class OpenGLImageView;
class OpenGLFramebuffer;

typedef OpenGLItemPtr<OpenGLImageView> OpenGLImageViewPtr;
typedef OpenGLItemPtr<OpenGLFramebuffer> OpenGLFramebufferPtr;

class OpenGLFramebuffer
{
    friend OpenGLFramebufferPtr;

public:
    OpenGLFramebuffer(OpenGLImageViewPtr * ColorBuffers, uint32_t NoOfColorBuffers, OpenGLImageView * DepthBuffer, uint8_t * DrawBuffers, uint32_t NoOfDrawBuffers, uint32_t Width, uint32_t Height);
    ~OpenGLFramebuffer();

    void BindFramebuffer(GLenum Target);

private:
    OpenGLFramebuffer();
    OpenGLFramebuffer(const OpenGLFramebuffer&);
    OpenGLFramebuffer& operator=(const OpenGLFramebuffer&);

    void AttachTexture(GLenum attachment, const OpenGLImageView * image_view);
    static GLenum AttachmentType(SurfacePixelFormat format);

    GLuint m_Handle;
    GLbitfield m_BufferBits;
    uint32_t m_Ref;
};
