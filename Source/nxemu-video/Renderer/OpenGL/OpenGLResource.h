#pragma once
#include <glad/glad.h>

class OpenGLTexture 
{
public:
    OpenGLTexture();
    ~OpenGLTexture();

    void Create(GLenum target);
    void Release();

    void TextureStorage3D(GLsizei Levels, GLenum InternalFormat, GLsizei Width, GLsizei Height, GLsizei Depth);

private:
    OpenGLTexture(const OpenGLTexture&);
    OpenGLTexture & operator=(const OpenGLTexture &);
        
    GLuint m_Handle;
};
