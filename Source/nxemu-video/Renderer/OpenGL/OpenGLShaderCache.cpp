#include "OpenGLShaderCache.h"
#include "VideoNotification.h"

OpenGLShaderCache::OpenGLShaderCache(OpenGLRenderer & Renderer, CVideo & Video, const OpenGLDevice & Device) :
    m_Renderer(Renderer), 
    m_Video(Video), 
    m_Device(Device) 
{
}

OpenGLShaderCache::~OpenGLShaderCache() 
{
}

OpenGLCompiledShaderPtr OpenGLShaderCache::GetStageProgram(CMaxwell3D::ShaderProgram /*ProgramType*/) 
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}
