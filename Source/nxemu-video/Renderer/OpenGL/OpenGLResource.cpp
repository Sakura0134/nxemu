#include "OpenGLResource.h"
#include "MaxwellToOpenGL.h"
#include "VideoNotification.h"
#include <string>

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

void OpenGLTexture::BindTexture(GLuint First)
{
    if (m_Handle == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glBindTextures(First, 1, &m_Handle);
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

OpenGLSampler::OpenGLSampler(const TextureTSCEntry & Config) : 
    m_Handle(0),
    m_Ref(0) 
{
    GLenum CompareMode = Config.DepthCompareEnabled ? GL_COMPARE_REF_TO_TEXTURE : GL_NONE;
    GLenum CompareFunc = MaxwellToOpenGL_DepthCompareFunc(Config.DepthCompareFunc);
    GLenum Mag = MaxwellToOpenGL_TextureFilterMode(Config.MagFilter, TextureMipmapFilter_None);
    GLenum Min = MaxwellToOpenGL_TextureFilterMode(Config.MinFilter, Config.MipmapFilter);
    GLenum ReductionFilter = MaxwellToOpenGL_ReductionFilter(Config.ReductionFilter);
    GLint Seamless = Config.CubemapInterfaceFiltering ? GL_TRUE : GL_FALSE;

    if (Config.CubemapAnisotropy != 1 || Config.FloatCoordNormalization != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    Create();
    glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_S, MaxwellToOpenGL_WrapMode(Config.WrapU));
    glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_T, MaxwellToOpenGL_WrapMode(Config.WrapV));
    glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_R, MaxwellToOpenGL_WrapMode(Config.WrapP));
    glSamplerParameteri(m_Handle, GL_TEXTURE_COMPARE_MODE, CompareMode);
    glSamplerParameteri(m_Handle, GL_TEXTURE_COMPARE_FUNC, CompareFunc);
    glSamplerParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, Mag);
    glSamplerParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, Min);
    glSamplerParameterf(m_Handle, GL_TEXTURE_LOD_BIAS, Config.LodBias());
    glSamplerParameterf(m_Handle, GL_TEXTURE_MIN_LOD, Config.MinLod());
    glSamplerParameterf(m_Handle, GL_TEXTURE_MAX_LOD, Config.MaxLod());
    glSamplerParameterfv(m_Handle, GL_TEXTURE_BORDER_COLOR, Config.BorderColor());

    if (GLAD_GL_ARB_texture_filter_anisotropic || GLAD_GL_EXT_texture_filter_anisotropic) 
    {
        glSamplerParameterf(m_Handle, GL_TEXTURE_MAX_ANISOTROPY, 1U);
    }
    if (GLAD_GL_ARB_texture_filter_minmax || GLAD_GL_EXT_texture_filter_minmax)
    {
        glSamplerParameteri(m_Handle, GL_TEXTURE_REDUCTION_MODE_ARB, ReductionFilter);
    }
    if (GLAD_GL_ARB_seamless_cubemap_per_texture || GLAD_GL_AMD_seamless_cubemap_per_texture) 
    {
        glSamplerParameteri(m_Handle, GL_TEXTURE_CUBE_MAP_SEAMLESS, Seamless);
    }
}

OpenGLSampler::~OpenGLSampler()
{
    Release();
}

void OpenGLSampler::Create() 
{
    if (m_Handle != 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glCreateSamplers(1, &m_Handle);
}

void OpenGLSampler::Release()
{
    if (m_Handle == 0) 
    {
        return;
    }

    glDeleteSamplers(1, &m_Handle);
    m_Handle = 0;
}

void OpenGLSampler::BindTexture(GLuint First)
{
    if (m_Handle == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glBindImageTextures(First, 1, &m_Handle);
}

OpenGLShader::OpenGLShader() :
    m_Handle(0)
{
} 

OpenGLShader::~OpenGLShader() 
{
    Release();
}

void OpenGLShader::Create(const char * Source, GLenum Type) 
{
    if (m_Handle != 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    if (Source == nullptr || Source[0] == '\0')
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_Handle = glCreateShader(Type);

    const GLchar * SourceString = Source;
    GLint SourceLength = (GLint)strlen(Source);

    glShaderSource(m_Handle, 1, &SourceString, &SourceLength);
    glCompileShader(m_Handle);

    GLint InfoLogLength;
    glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 1)
    {
        GLint Result = GL_FALSE;
        glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &Result);
        std::string ShaderError(InfoLogLength, ' ');
        glGetShaderInfoLog(m_Handle, InfoLogLength, nullptr, &ShaderError[0]);
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void OpenGLShader::Release()
{
    if (m_Handle == 0) 
    {
        return;        
    }

    glDeleteShader(m_Handle);
    m_Handle = 0;
}

OpenGLProgram::OpenGLProgram() :
    m_Handle(0),
    m_Ref(0)
{
} 

OpenGLProgram::~OpenGLProgram() 
{
    Release();
}

void OpenGLProgram::Create(bool SeparableProgram, bool HintRetrievable, const OpenGLShader & Shader) 
{
    if (m_Handle != 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    GLuint ProgramId = glCreateProgram();
    glAttachShader(ProgramId, Shader.m_Handle);

    if (SeparableProgram)
    {
        glProgramParameteri(ProgramId, GL_PROGRAM_SEPARABLE, GL_TRUE);
    }
    if (HintRetrievable)
    {
        glProgramParameteri(ProgramId, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
    }
    glLinkProgram(ProgramId);

    GLint InfoLogLength;
    glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 1)
    {
        GLint result = GL_FALSE;
        glGetProgramiv(ProgramId, GL_LINK_STATUS, &result);
        std::string ProgramError(InfoLogLength, ' ');
        glGetProgramInfoLog(ProgramId, InfoLogLength, nullptr, &ProgramError[0]);
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    glDetachShader(ProgramId, Shader.m_Handle);
    m_Handle = ProgramId;
}

void OpenGLProgram::Release() 
{
    if (m_Handle == 0) 
    {
        return;    
    }
    glDeleteProgram(m_Handle);
    m_Handle = 0;
}

OpenGLPipeline::OpenGLPipeline() : 
    m_Handle(0) 
{
}

OpenGLPipeline::~OpenGLPipeline()
{
    Release();
}

void OpenGLPipeline::Create() 
{
    if (m_Handle != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    glGenProgramPipelines(1, &m_Handle);
}

void OpenGLPipeline::Release() 
{
    if (m_Handle == 0)
    {
        return;
    }
    glDeleteProgramPipelines(1, &m_Handle);
    m_Handle = 0;
}

void OpenGLPipeline::BindProgramPipeline() const 
{
    if (m_Handle == 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glBindProgramPipeline(m_Handle);
}

void OpenGLPipeline::UseProgramStages(GLbitfield Stages, const OpenGLProgram & Program) const
{
    if (m_Handle == 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glUseProgramStages(m_Handle, Stages, Program.m_Handle);
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

void OpenGLBuffer::BindBufferRange(GLenum Target, GLuint Index, GLintptr Offset, GLsizeiptr Size) const
{
    if (m_Handle == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glBindBufferRange(Target, Index, m_Handle, Offset, Size);
}

void OpenGLBuffer::BindBufferRangeNV(GLenum Target, GLuint Index, GLintptr Offset, GLsizeiptr Size) const
{
    if (m_Handle == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glBindBufferRangeNV(Target, Index, m_Handle, Offset, Size);
}

void OpenGLBuffer::BindVertexBuffer(GLuint BindingIndex, GLintptr Offset, GLsizei Stride) const
{
    if (m_Handle == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glBindVertexBuffer(BindingIndex, m_Handle, Offset, Stride);
}

void OpenGLBuffer::FlushMappedNamedBufferRange(GLintptr Offset, GLsizeiptr Length) const
{
    if (m_Handle == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glFlushMappedNamedBufferRange(m_Handle, Offset, Length);
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

void OpenGLSync::GetSynciv(GLenum PName, GLsizei Count, GLsizei * Length, GLint * Values) const
{
    if (m_Handle == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    glGetSynciv(m_Handle, PName, Count, Length, Values);
}
