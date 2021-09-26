#pragma once
#include "OpenGLResource.h"

class OpenGLDevice;

class OpenGLProgramManager
{
public:
    OpenGLProgramManager(void);
    ~OpenGLProgramManager();

    bool Init(const OpenGLDevice & device);
    void UseVertexShader(OpenGLProgramPtr & Program);
    void UseGeometryShader(OpenGLProgramPtr & Program);
    void UseFragmentShader(OpenGLProgramPtr & Program);

private:
    OpenGLProgramManager(const OpenGLProgramManager&);
    OpenGLProgramManager& operator=(const OpenGLProgramManager&);

    struct PipelineState 
    {
        OpenGLProgramPtr Vertex;
        OpenGLProgramPtr Geometry;
        OpenGLProgramPtr Fragment;
    };

    static void BindProgram(GLenum Stage, const OpenGLProgramPtr & Current, const OpenGLProgramPtr & Old, bool & Enabled);

    OpenGLPipeline m_GraphicsPipeline;
    PipelineState m_CurrentState;
    bool m_UseAssemblyShaders;
    bool m_VertexEnabled;
    bool m_GeometryEnabled;
    bool m_FragmentEnabled;
};