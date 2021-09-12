#include "OpenGLProgramManager.h"
#include "OpenGLDevice.h"
#include "VideoNotification.h"

OpenGLProgramManager::OpenGLProgramManager(void) : 
    m_UseAssemblyShaders(false), 
    m_VertexEnabled(false),
    m_GeometryEnabled(false),
    m_FragmentEnabled(false)
{
}

OpenGLProgramManager::~OpenGLProgramManager(void)
{
}

bool OpenGLProgramManager::Init(const OpenGLDevice& device) 
{
    m_UseAssemblyShaders = device.UseAssemblyShaders();
    if (m_UseAssemblyShaders) 
    {
        glEnable(GL_COMPUTE_PROGRAM_NV);
    } 
    else 
    {
        m_GraphicsPipeline.Create();
        m_GraphicsPipeline.BindProgramPipeline();
    }
    return true;
}

void OpenGLProgramManager::UseVertexShader(OpenGLProgramPtr & Program)
{
    if (m_UseAssemblyShaders)
    {
        BindProgram(GL_VERTEX_PROGRAM_NV, Program, m_CurrentState.Vertex, m_VertexEnabled);
    }
    m_CurrentState.Vertex = Program;
}

void OpenGLProgramManager::UseGeometryShader(OpenGLProgramPtr & Program)
{
    if (m_UseAssemblyShaders)
    {
        BindProgram(GL_GEOMETRY_PROGRAM_NV, Program, m_CurrentState.Vertex, m_GeometryEnabled);
    }
    m_CurrentState.Geometry = Program;
}

void OpenGLProgramManager::UseFragmentShader(OpenGLProgramPtr & Program)
{
    if (m_UseAssemblyShaders)
    {
        BindProgram(GL_FRAGMENT_PROGRAM_NV, Program, m_CurrentState.Vertex, m_FragmentEnabled);
    }
    m_CurrentState.Fragment = Program;
}

void OpenGLProgramManager::BindProgram(GLenum /*Stage*/, const OpenGLProgramPtr & Current, const OpenGLProgramPtr & Old, bool & /*Enabled*/)
{
    if (Current == Old)
    {
        return;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}
