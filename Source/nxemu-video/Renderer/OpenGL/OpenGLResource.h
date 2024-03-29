#pragma once
#include "OpenGLItemPtr.h"
#include "Textures\Texture.h"
#include <glad\glad.h>
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

    void BindTexture(GLuint First);
    void BindTextureUnit(GLuint unit);
    void ClearTexImage(GLint Level, GLenum Format, GLenum Type, const void * Data);
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

class OpenGLSampler;
typedef OpenGLItemPtr<OpenGLSampler> OpenGLSamplerPtr;

class OpenGLSampler 
{
    friend OpenGLSamplerPtr;

public:
    OpenGLSampler();
    OpenGLSampler(const TextureTSCEntry & Config);
    ~OpenGLSampler();

    void Create();
    void Release();

    void BindSampler(GLuint Unit);
    void BindTexture(GLuint First);
    void SamplerParameteri(GLenum pname, GLint param);

private:
    OpenGLSampler(const OpenGLSampler&);
    OpenGLSampler& operator=(const OpenGLSampler&);

    GLuint m_Handle;
    int32_t m_Ref;
};

class OpenGLProgram;

class OpenGLShader 
{
    friend OpenGLProgram;

public:
    OpenGLShader();
    ~OpenGLShader();

    void Create(const char * Source, GLenum Type);
    void Release();

private:
    OpenGLShader(const OpenGLShader&);
    OpenGLShader& operator=(const OpenGLShader&);

    GLuint m_Handle;
};

class OpenGLPipeline;
typedef OpenGLItemPtr<OpenGLProgram> OpenGLProgramPtr;

class OpenGLProgram 
{
    friend OpenGLProgramPtr;
    friend OpenGLPipeline;

public:
    OpenGLProgram();
    ~OpenGLProgram();

    void Create(bool separable_program, bool hint_retrievable, const OpenGLShader & Shader);
    void Release();

    bool Created() const { return m_Handle != 0; }

    void ProgramUniformMatrix3x2fv(GLint Location, GLsizei Count, GLboolean Transpose, const GLfloat * Value);

private:
    OpenGLProgram(const OpenGLProgram&);
    OpenGLProgram& operator=(const OpenGLProgram&);

    GLuint m_Handle;
    uint32_t m_Ref;
};

class OpenGLPipeline 
{
public:
    OpenGLPipeline();
    ~OpenGLPipeline();

    void Create();
    void Release();
    
    void BindProgramPipeline(void) const;
    void UseProgramStages(GLbitfield Stages, const OpenGLProgram & Program) const;

private:
    OpenGLPipeline(const OpenGLPipeline&);
    OpenGLPipeline& operator=(const OpenGLPipeline&);

    GLuint m_Handle;
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
    void BindBufferRange(GLenum Target, GLuint Index, GLintptr Offset, GLsizeiptr Size) const;
    void BindBufferRangeNV(GLenum Target, GLuint Index, GLintptr Offset, GLsizeiptr Size) const;
    void BindVertexBuffer(GLuint BindingIndex, GLintptr Offset, GLsizei Stride) const;
    void FlushMappedNamedBufferRange(GLintptr Offset, GLsizeiptr Length) const;
    void NamedBufferData(GLsizeiptr Size, const void* Data, GLenum Usage);
    void NamedBufferSubData(GLintptr Offset, GLsizeiptr Size, const void * Data);
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
