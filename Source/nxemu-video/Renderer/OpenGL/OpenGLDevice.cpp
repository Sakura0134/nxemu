#include "OpenGLDevice.h"
#include <glad/glad.h>

OpenGLDevice::OpenGLDevice() : 
    m_UniformBufferAlignment(4),
    m_HasVertexBufferUnifiedMemory(false),
    m_UseAssemblyShaders(false)
{
    memset(m_MaxUniformBuffers, 0, sizeof(m_MaxUniformBuffers));
}

bool OpenGLDevice::Init(void)
{
    BuildBaseBindings();
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &m_UniformBufferAlignment);
    m_HasVertexBufferUnifiedMemory = GLAD_GL_NV_vertex_buffer_unified_memory != 0;
    m_UseAssemblyShaders = GLAD_GL_NV_gpu_program5 && GLAD_GL_NV_compute_program5 && GLAD_GL_NV_transform_feedback && GLAD_GL_NV_transform_feedback2;
    return true;
}

void OpenGLDevice::BuildBaseBindings() 
{
    GLenum LimitUniforBuffer[] =
    {
        GL_MAX_VERTEX_UNIFORM_BLOCKS,
        GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS,
        GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS,
        GL_MAX_GEOMETRY_UNIFORM_BLOCKS,
        GL_MAX_FRAGMENT_UNIFORM_BLOCKS,
        GL_MAX_COMPUTE_UNIFORM_BLOCKS,
    };

    for (size_t i = 0, n = sizeof(LimitUniforBuffer) / sizeof(LimitUniforBuffer[0]); i < n; i++) 
    {
        GLint MaxUniformBuffer;
        glGetIntegerv(LimitUniforBuffer[i], &MaxUniformBuffer);
        m_MaxUniformBuffers[i] = (uint32_t)MaxUniformBuffer;
    }
}
