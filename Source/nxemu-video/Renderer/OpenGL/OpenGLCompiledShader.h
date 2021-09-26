#pragma once
#include "OpenGLResource.h"
#include "OpenGLItemPtr.h"
#include "OpenGLShaderDecompiler.h"
#include "Engine\Maxwell3D.h"
#include "Shader\ShaderNode.h"

class CVideo;
class OpenGLDevice;
class ShaderIR;
__interface IRenderer;

class OpenGLCompiledShader;
typedef OpenGLItemPtr<OpenGLCompiledShader> OpenGLCompiledShaderPtr;

class OpenGLCompiledShader
{
    friend OpenGLCompiledShaderPtr;

public:
    ~OpenGLCompiledShader();

    static OpenGLCompiledShaderPtr CreateStageFromMemory(IRenderer & Renderer, CVideo & Video, const OpenGLDevice & Device, CMaxwell3D::ShaderProgram ProgramType);

    uint64_t Address() const { return m_Address; }
    uint32_t Size() const { return m_Size; }
    const OpenGLShaderConstBufferList & ConstBuffers() const { return m_ConstBuffers; }
    const ShaderSamplerEntryList & Samplers() const { return m_Samplers; }
    bool UseUnifiedUniforms() const { return m_UseUnifiedUniforms; }
    OpenGLProgramPtr & Program() { return m_Program; }
    bool IsBuilt() const { return m_IsBuilt; }

private:
    OpenGLCompiledShader(void);
    OpenGLCompiledShader(const OpenGLCompiledShader&);
    OpenGLCompiledShader& operator=(const OpenGLCompiledShader&);

    OpenGLCompiledShader(const OpenGLDevice & Device, const ShaderIR & IR, ShaderType Stage, uint64_t Addr, uint32_t Size, OpenGLProgramPtr & Program, bool IsBuilt);

    static const char * GetShaderTypeName(ShaderType Type);
    static GLenum GetGLShaderType(ShaderType Type);

    uint64_t m_Address;
    uint32_t m_Size;
    OpenGLShaderConstBufferList m_ConstBuffers;
    ShaderSamplerEntryList m_Samplers;
    bool m_UseUnifiedUniforms;
    OpenGLProgramPtr m_Program;
    bool m_IsBuilt;
    uint32_t m_Ref;
};
