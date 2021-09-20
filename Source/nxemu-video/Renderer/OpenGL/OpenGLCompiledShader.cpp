#include "OpenGLCompiledShader.h"
#include "OpenGLShaderDecompiler.h"
#include "Shader/Shader.h"
#include "Shader/ShaderIR.h"
#include "Video.h"
#include "OpenGLDevice.h"
#include "VideoNotification.h"
#include <Common/StdString.h>

OpenGLCompiledShader::OpenGLCompiledShader(const OpenGLDevice & Device, const ShaderIR & IR, ShaderType Stage, uint64_t Address, uint32_t Size, OpenGLProgramPtr & Program, bool IsBuilt) :
    m_Address(Address),
    m_Size(Size),
    m_UseUnifiedUniforms(IR.GetConstantBuffers().size() > Device.GetMaxUniformBuffers(Stage) - 1),
    m_Program(Program),
    m_IsBuilt(IsBuilt),
    m_Ref(0)
{
    if (m_IsBuilt && !m_Program->Created())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    const ShaderConstBufferMap & ConstBuffers = IR.GetConstantBuffers();
    for (ShaderConstBufferMap::const_iterator itr = ConstBuffers.begin(); itr != ConstBuffers.end(); itr++) 
    {
        m_ConstBuffers.emplace_back(itr->second.GetMaxOffset(), itr->second.IsIndirect(), itr->first);
    }

    const ShaderSamplerEntryList & Samplers = IR.GetSamplers();
    for (ShaderSamplerEntryList::const_iterator itr = Samplers.begin(); itr != Samplers.end(); itr++) 
    {
        m_Samplers.emplace_back(*itr);
    }
}

OpenGLCompiledShader::~OpenGLCompiledShader()
{
}

OpenGLCompiledShaderPtr OpenGLCompiledShader::CreateStageFromMemory(IRenderer & /*Renderer*/, CVideo & Video, const OpenGLDevice & Device, CMaxwell3D::ShaderProgram ProgramType)
{
    const CMaxwell3D::Registers & Regs = Video.Maxwell3D().Regs();
    const CMaxwell3D::tyShaderConfig & ShaderConfig = Regs.ShaderConfig[ProgramType];
    uint64_t Address = Regs.CodeAddress.Address() + ShaderConfig.Offset;

    ShaderProgramCode ShaderCode = GetShaderCode(Video.VideoMemory(), Address, false), ShaderCodeB;
    if (ProgramType == CMaxwell3D::ShaderProgram_VertexA) 
    {
        const CMaxwell3D::tyShaderConfig & ShaderConfigVertexB = Regs.ShaderConfig[CMaxwell3D::ShaderProgram_VertexB];
        uint64_t AddressB = Regs.CodeAddress.Address() + ShaderConfigVertexB.Offset;
        ShaderCodeB = GetShaderCode(Video.VideoMemory(), AddressB, false);
    }
    uint64_t UniqueIdentifier = ShaderUniqueIdentifier(ShaderCode, ShaderCodeB, ProgramType == CMaxwell3D::ShaderProgram_VertexA);
    uint32_t ShaderCodeSize = (uint32_t)(ShaderCode.size() * sizeof(uint64_t));

    ShaderType Type = CMaxwell3D::GetShaderType(ProgramType);
    ShaderIR IR(ShaderCode);
    std::string ShaderId = stdstr_f("%s:%016llX", GetShaderTypeName(Type), UniqueIdentifier);
    std::string glsl = OpenGLDecompileShader(Device, IR, Type, ShaderId.c_str());
    OpenGLShader Shader;
    Shader.Create(glsl.c_str(), GetGLShaderType(Type));
    OpenGLProgramPtr Program(new OpenGLProgram);
    Program->Create(true, false, Shader);
    return OpenGLCompiledShaderPtr(new OpenGLCompiledShader(Device, IR, Type, Address, ShaderCodeSize, Program, true));
}

const char * OpenGLCompiledShader::GetShaderTypeName(ShaderType Type)
{
    switch (Type)
    {
    case ShaderType_Vertex: return "VS";
    case ShaderType_TesselationControl: return "HS";
    case ShaderType_TesselationEval: return "DS";
    case ShaderType_Geometry: return "GS";
    case ShaderType_Fragment: return "FS";
    case ShaderType_Compute: return "CS";
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return "UNK";
}

GLenum OpenGLCompiledShader::GetGLShaderType(ShaderType Type)
{
    switch (Type)
    {
    case ShaderType_Vertex: return GL_VERTEX_SHADER;
    case ShaderType_Geometry: return GL_GEOMETRY_SHADER;
    case ShaderType_Fragment: return GL_FRAGMENT_SHADER;
    case ShaderType_Compute: return GL_COMPUTE_SHADER;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return GL_NONE;
}
