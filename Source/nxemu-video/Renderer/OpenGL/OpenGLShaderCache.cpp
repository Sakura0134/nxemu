#include "OpenGLShaderCache.h"
#include "OpenGLStateTracker.h"
#include "OpenGLRenderer.h"
#include "Video.h"
#include "VideoNotification.h"

OpenGLShaderCache::OpenGLShaderCache(OpenGLRenderer & Renderer, CVideo & Video, const OpenGLDevice & Device) :
    m_Renderer(Renderer), 
    m_Video(Video), 
    m_VideoMemory(Video.VideoMemory()),
    m_Maxwell3D(Video.Maxwell3D()), 
    m_Device(Device) 
{
}

OpenGLShaderCache::~OpenGLShaderCache() 
{
}

OpenGLCompiledShaderPtr OpenGLShaderCache::GetStageProgram(CMaxwell3D::ShaderProgram ProgramType) 
{
    if (!m_Maxwell3D.StateTracker().Flag(OpenGLDirtyFlag_Shaders))
    {
        OpenGLCompiledShaderPtr & LastShader = m_LastShaders[ProgramType];
        if (LastShader->IsBuilt()) 
        {
            return LastShader;
        }
    }

    const CMaxwell3D::tyShaderConfig & ShaderConfig = m_Maxwell3D.Regs().ShaderConfig[ProgramType];
    uint64_t Address = m_Maxwell3D.Regs().CodeAddress.Address() + ShaderConfig.Offset;

    uint64_t CpuAddr;
    if (!m_VideoMemory.GpuToCpuAddress(Address, CpuAddr) || CpuAddr == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }

    OpenGLCompiledShaderPtr Shader;
    ShaderAddrMap::const_iterator itr = m_ShaderAddrMap.find(CpuAddr);
    if (itr != m_ShaderAddrMap.end())
    {
        Shader = itr->second;
    }
    if (Shader.Get() != nullptr) 
    {
        m_LastShaders[ProgramType] = Shader;
        return Shader;
    }

    Shader = OpenGLCompiledShader::CreateStageFromMemory(m_Renderer, m_Video, m_Device, ProgramType);
    m_ShaderAddrMap.emplace(Shader->Address(), Shader);
    m_Shaders.push_back(Shader);

    uint64_t PageEnd = ((CpuAddr + Shader->Size()) + PAGE_SIZE - 1) >> PAGE_BITS;
    for (uint64_t Page = CpuAddr >> PAGE_BITS; Page < PageEnd; Page++) 
    {
        m_ShaderPageMap[Page].push_back(Shader);
    }
    m_Renderer.TrackRasterizerMemory(CpuAddr, Shader->Size(), true);
    m_LastShaders[ProgramType] = Shader;
    return Shader;
}
