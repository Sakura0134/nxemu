#pragma once
#include <glad\glad.h>

class OpenGLDevice 
{
public:

    OpenGLDevice();

    GLint GetUniformBufferAlignment() const { return m_UniformBufferAlignment; }
    bool HasVertexBufferUnifiedMemory() const { return m_HasVertexBufferUnifiedMemory; }
    bool UseAssemblyShaders() const { return m_UseAssemblyShaders; }
    bool Init(void);

private:
    OpenGLDevice(const OpenGLDevice&);
    OpenGLDevice& operator=(const OpenGLDevice&);

    GLint m_UniformBufferAlignment;
    bool m_HasVertexBufferUnifiedMemory;
    bool m_UseAssemblyShaders;
};
