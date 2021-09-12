#pragma once
#include "OpenGLCompiledShader.h"
#include "Engine\Maxwell3D.h"

class CVideo;
class OpenGLDevice;
class OpenGLRenderer;

class OpenGLShaderCache
{
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
    const OpenGLDevice & m_Device;
};