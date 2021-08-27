#pragma once
#include "OpenGLItemPtr.h"
#include <glad/glad.h>
#include <stdint.h>

class OpenGLFramebuffer;

class OpenGLTexture;
typedef OpenGLItemPtr<OpenGLTexture> OpenGLTexturePtr;

class OpenGLTexture 
{
    friend OpenGLTexturePtr;
    friend OpenGLFramebuffer;

public:
    OpenGLTexture();
    ~OpenGLTexture();

    void Create(GLenum target);
    void Generate();
    void Release();

    void TextureView(GLenum Target, const OpenGLTexture & OrigTexture, GLenum InternalFormat, GLuint MinLevel, GLuint NumLevels, GLuint MinLayer, GLuint NumLayers);
    void TextureStorage2D(GLsizei Levels, GLenum InternalFormat, GLsizei Width, GLsizei Height);
    void TextureStorage3D(GLsizei Levels, GLenum InternalFormat, GLsizei Width, GLsizei Height, GLsizei Depth);
    void TextureSubImage3D(GLint Level, GLint XOffset, GLint YOffset, GLint ZOffset, GLsizei Width, GLsizei Height, GLsizei Depth, GLenum Format, GLenum Type, const void * Pixels);
    void TextureParameteriv(GLenum PName, const GLint * Param);

private:
    OpenGLTexture(const OpenGLTexture&);
    OpenGLTexture & operator=(const OpenGLTexture &);
        
    GLuint m_Handle;
    int32_t m_Ref;
};

class OpenGLBuffer;
typedef OpenGLItemPtr<OpenGLBuffer> OpenGLBufferPtr;

class OpenGLBuffer
{
    friend OpenGLBufferPtr;

public:
    OpenGLBuffer();
    ~OpenGLBuffer();

    void Create();
    void Release();

    void BindBuffer(GLenum Target) const;
    void BindVertexBuffer(GLuint BindingIndex, GLintptr Offset, GLsizei Stride) const;
    void NamedBufferStorage(GLsizeiptr Size, const void * Data, GLbitfield Flags);
    void * MapNamedBufferRange(GLintptr Offset, GLsizeiptr length, GLbitfield access);

private:
    OpenGLBuffer(const OpenGLBuffer&);
    OpenGLBuffer& operator=(const OpenGLBuffer&);

    GLuint m_Handle;
    uint32_t m_Ref;
};

class OpenGLSync;
typedef OpenGLItemPtr<OpenGLSync> OpenGLSyncPtr;

class OpenGLSync
{
    friend OpenGLSyncPtr;

public:
    OpenGLSync();
    ~OpenGLSync();

    void Release();

    void GetSynciv(GLenum PName, GLsizei Count, GLsizei * Length, GLint * Values) const;
    bool Created() const { return m_Handle != 0; }

private:
    OpenGLSync(const OpenGLSync&);
    OpenGLSync& operator=(const OpenGLSync&);

    GLsync m_Handle;
    uint32_t m_Ref;
};
