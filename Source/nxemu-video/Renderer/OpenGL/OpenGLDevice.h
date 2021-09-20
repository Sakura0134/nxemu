#pragma once
#include "GpuTypes.h"
#include "Shader\Shader.h"
#include <glad\glad.h>

class OpenGLDevice 
{
public:

    OpenGLDevice();

    uint32_t GetMaxUniformBuffers(ShaderType Type) const { return m_MaxUniformBuffers[Type]; }
    GLint GetUniformBufferAlignment() const { return m_UniformBufferAlignment; }
    bool HasVertexBufferUnifiedMemory() const { return m_HasVertexBufferUnifiedMemory; }
    bool UseAssemblyShaders() const { return m_UseAssemblyShaders; }
    bool Init(void);

private:
    OpenGLDevice(const OpenGLDevice&);
    OpenGLDevice& operator=(const OpenGLDevice&);

    void BuildBaseBindings();

    uint32_t m_MaxUniformBuffers[MaxShaderTypes];
    GLint m_UniformBufferAlignment;
    bool m_HasVertexBufferUnifiedMemory;
    bool m_UseAssemblyShaders;
};
