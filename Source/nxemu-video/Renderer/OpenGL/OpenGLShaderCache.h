#pragma once
#include "OpenGLCompiledShader.h"
#include "Engine\Maxwell3D.h"
#include <vector>
#include <unordered_map>
#include <stdint.h>

class CVideo;
class OpenGLDevice;
class OpenGLRenderer;

class OpenGLShaderCache
{
    enum : uint64_t
    {
        PAGE_BITS = 14,
        PAGE_SIZE = 1 << PAGE_BITS,
    };

    typedef std::vector<OpenGLCompiledShaderPtr> ShaderList;
    typedef std::unordered_map<uint64_t, OpenGLCompiledShaderPtr> ShaderAddrMap;
    typedef std::unordered_map<uint64_t, ShaderList> ShaderAddrListMap;

public:
    OpenGLShaderCache(OpenGLRenderer & Renderer, CVideo & Video, const OpenGLDevice & Device);
    ~OpenGLShaderCache();

    OpenGLCompiledShaderPtr GetStageProgram(CMaxwell3D::ShaderProgram ProgramType);

private:
    OpenGLShaderCache(void);
    OpenGLShaderCache(const OpenGLShaderCache&);
    OpenGLShaderCache& operator=(const OpenGLShaderCache&);

    OpenGLRenderer & m_Renderer;
    CVideo & m_Video;
    CVideoMemory & m_VideoMemory;
    CMaxwell3D & m_Maxwell3D;
    const OpenGLDevice & m_Device;
    ShaderAddrMap m_ShaderAddrMap;
    ShaderAddrListMap m_ShaderPageMap;
    ShaderList m_Shaders;
    OpenGLCompiledShaderPtr m_LastShaders[CMaxwell3D::MaxShaderProgram];
};