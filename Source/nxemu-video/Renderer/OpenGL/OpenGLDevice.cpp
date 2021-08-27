#include "OpenGLDevice.h"
#include <glad/glad.h>

OpenGLDevice::OpenGLDevice() : 
    m_UniformBufferAlignment(4),
    m_HasVertexBufferUnifiedMemory(false),
    m_UseAssemblyShaders(false)
{
}

bool OpenGLDevice::Init(void)
{
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &m_UniformBufferAlignment);
    m_HasVertexBufferUnifiedMemory = GLAD_GL_NV_vertex_buffer_unified_memory != 0;
    m_UseAssemblyShaders = GLAD_GL_NV_gpu_program5 && GLAD_GL_NV_compute_program5 && GLAD_GL_NV_transform_feedback && GLAD_GL_NV_transform_feedback2;
    return true;
}