#include "OpenGLResource.h"
#include "VideoNotification.h"

OpenGLTexture::OpenGLTexture() : 
    m_Handle(0),
    m_Ref(0) 
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

void OpenGLTexture::Generate() 
{
    if (m_Handle != 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glGenTextures(1, &m_Handle);
}

void OpenGLTexture::Release() 
{
    if (m_Handle == 0)
        return;

    glDeleteTextures(1, &m_Handle);
    m_Handle = 0;
}

void OpenGLTexture::TextureView(GLenum Target, const OpenGLTexture & OrigTexture, GLenum InternalFormat, GLuint MinLevel, GLuint NumLevels, GLuint MinLayer, GLuint NumLayers)
{
    if (m_Handle == 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glTextureView(m_Handle, Target, OrigTexture.m_Handle, InternalFormat, MinLevel, NumLevels, MinLayer, NumLayers);
}

void OpenGLTexture::TextureStorage2D(GLsizei Levels, GLenum InternalFormat, GLsizei Width, GLsizei Height)
{
    if (m_Handle == 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glTextureStorage2D(m_Handle, Levels, InternalFormat, Width, Height);
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

void OpenGLTexture::TextureSubImage3D(GLint Level, GLint XOffset, GLint YOffset, GLint ZOffset, GLsizei Width, GLsizei Height, GLsizei Depth, GLenum Format, GLenum Type, const void * Pixels)
{
    if (m_Handle == 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glTextureSubImage3D(m_Handle, Level, XOffset, YOffset, ZOffset, Width, Height, Depth, Format, Type, Pixels);
}

void OpenGLTexture::TextureParameteriv(GLenum PName, const GLint * Param) 
{
    if (m_Handle == 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glTextureParameteriv(m_Handle, PName, Param);
}

OpenGLBuffer::OpenGLBuffer() : 
    m_Handle(0), 
    m_Ref(0)
{
}

OpenGLBuffer::~OpenGLBuffer()
{
    Release();
}

void OpenGLBuffer::Create() 
{
    if (m_Handle != 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glCreateBuffers(1, &m_Handle);
}

void OpenGLBuffer::Release()
{
    if (m_Handle == 0) 
    {
        return;
    }
    glDeleteBuffers(1, &m_Handle);
    m_Handle = 0;
}

void OpenGLBuffer::BindBuffer(GLenum Target) const
{
    if (m_Handle == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glBindBuffer(Target, m_Handle);
}

void OpenGLBuffer::NamedBufferStorage(GLsizeiptr Size, const void * Data, GLbitfield Flags) 
{
    if (m_Handle == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glNamedBufferStorage(m_Handle, Size, Data, Flags);
}

void * OpenGLBuffer::MapNamedBufferRange(GLintptr Offset, GLsizeiptr length, GLbitfield access)
{
    if (m_Handle == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }
    return glMapNamedBufferRange(m_Handle, Offset, length, access);
}

OpenGLSync::OpenGLSync() :
    m_Handle(0),
    m_Ref(0)
{
}

OpenGLSync::~OpenGLSync()
{
    Release();
}

void OpenGLSync::Release()
{
    if (m_Handle != 0) 
    {
        glDeleteSync(m_Handle);
        m_Handle = 0;    
    }
}

