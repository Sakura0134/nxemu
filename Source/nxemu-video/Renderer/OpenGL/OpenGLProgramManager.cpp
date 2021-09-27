#include "OpenGLProgramManager.h"
#include "OpenGLDevice.h"
#include "VideoNotification.h"

OpenGLProgramManager::OpenGLProgramManager(void) : 
    m_UseAssemblyShaders(false), 
    m_IsGraphicsBound(true),
    m_VertexEnabled(false),
    m_GeometryEnabled(false),
    m_FragmentEnabled(false)
{
}

OpenGLProgramManager::~OpenGLProgramManager(void)
{
}

bool OpenGLProgramManager::Init(const OpenGLDevice & Device) 
{
    m_UseAssemblyShaders = Device.UseAssemblyShaders();
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

void OpenGLProgramManager::BindGraphicsPipeline() 
{
    if (!m_UseAssemblyShaders)
    {
        UpdateSourcePrograms();
    }
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

void OpenGLProgramManager::UpdateSourcePrograms()
{
    if (!m_IsGraphicsBound) 
    {
        m_IsGraphicsBound = true;
        glUseProgram(0);
    }

    if (m_CurrentState.Vertex != m_OldState.Vertex)
    {
        m_GraphicsPipeline.UseProgramStages(GL_VERTEX_SHADER_BIT, m_CurrentState.Vertex);
    }
    if (m_CurrentState.Geometry != m_OldState.Geometry)
    {
        m_GraphicsPipeline.UseProgramStages(GL_GEOMETRY_SHADER_BIT, m_CurrentState.Geometry);    
    }
    if (m_CurrentState.Fragment != m_OldState.Fragment)
    {
        m_GraphicsPipeline.UseProgramStages(GL_FRAGMENT_SHADER_BIT, m_CurrentState.Fragment);    
    }
    m_OldState = m_CurrentState;
}

void OpenGLProgramManager::BindProgram(GLenum /*Stage*/, const OpenGLProgramPtr & Current, const OpenGLProgramPtr & Old, bool & /*Enabled*/)
{
    if (Current == Old)
    {
        return;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
}
