#pragma once
#include "OpenGLResource.h"
#include "OpenGLItemPtr.h"

class OpenGLCompiledShader;
typedef OpenGLItemPtr<OpenGLCompiledShader> OpenGLCompiledShaderPtr;

class OpenGLCompiledShader
{
    friend OpenGLCompiledShaderPtr;

public:
    ~OpenGLCompiledShader();

    OpenGLProgramPtr & Program() { return m_Program; }
    bool IsBuilt() const { return m_IsBuilt; }

private:
    OpenGLCompiledShader(void);
    OpenGLCompiledShader(const OpenGLCompiledShader&);
    OpenGLCompiledShader& operator=(const OpenGLCompiledShader&);

    OpenGLProgramPtr m_Program;
    bool m_IsBuilt;
    uint32_t m_Ref;
};
