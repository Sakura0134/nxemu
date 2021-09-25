#include "OpenGLDevice.h"
#include "VideoNotification.h"
#include <glad/glad.h>
#include <algorithm>

OpenGLDevice::OpenGLDevice() : 
    m_UniformBufferAlignment(4),
    m_HasVertexBufferUnifiedMemory(false),
    m_HasPreciseBug(false),
    m_UseAssemblyShaders(false),
    m_HasWarpIntrinsics(false),
    m_HasShaderBallot(false),
    m_HasVertexViewportLayer(false), 
    m_HasImageLoadFormatted(false),
    m_HasTextureShadowLOD(false) 
{
    memset(m_MaxUniformBuffers, 0, sizeof(m_MaxUniformBuffers));
    memset(m_BaseBindings, 0, sizeof(m_BaseBindings));
}

bool OpenGLDevice::Init(void)
{
    BuildBaseBindings();
    GetExtensions();
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &m_UniformBufferAlignment);
    m_HasWarpIntrinsics = GLAD_GL_NV_gpu_shader5 && GLAD_GL_NV_shader_thread_group && GLAD_GL_NV_shader_thread_shuffle;
    m_HasShaderBallot = GLAD_GL_ARB_shader_ballot != 0;
    m_HasVertexViewportLayer = GLAD_GL_ARB_shader_viewport_layer_array != 0;
    m_HasImageLoadFormatted = HasExtension("GL_EXT_shader_image_load_formatted");
    m_HasTextureShadowLOD = HasExtension("GL_EXT_texture_shadow_lod");
    m_HasPreciseBug = TestPreciseBug();
    m_HasVertexBufferUnifiedMemory = GLAD_GL_NV_vertex_buffer_unified_memory != 0;
    m_UseAssemblyShaders = GLAD_GL_NV_gpu_program5 && GLAD_GL_NV_compute_program5 && GLAD_GL_NV_transform_feedback && GLAD_GL_NV_transform_feedback2;
    return true;
}

void OpenGLDevice::BuildBaseBindings() 
{
    enum 
    {
        NumStages = 5,
        ReservedUniformBlocks = 1,
    };

    GLenum LimitImages[] = 
    {
        GL_MAX_VERTEX_IMAGE_UNIFORMS,
        GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS,
        GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS,
        GL_MAX_GEOMETRY_IMAGE_UNIFORMS,
        GL_MAX_FRAGMENT_IMAGE_UNIFORMS,
        GL_MAX_COMPUTE_IMAGE_UNIFORMS,
    };

    GLenum LimitSamplers[] =
    {
        GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
        GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS,
        GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS,
        GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS,
        GL_MAX_TEXTURE_IMAGE_UNITS,
        GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS,
    };

    GLenum LimitStorageBuffer[] = 
    {
        GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS,
        GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS,
        GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS,
        GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS,
        GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS,
        GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS,
    };

    GLenum LimitUniforBuffer[] =
    {
        GL_MAX_VERTEX_UNIFORM_BLOCKS,
        GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS,
        GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS,
        GL_MAX_GEOMETRY_UNIFORM_BLOCKS,
        GL_MAX_FRAGMENT_UNIFORM_BLOCKS,
        GL_MAX_COMPUTE_UNIFORM_BLOCKS,
    };

    uint32_t TotalUniforBufferBindings = (uint32_t)GL_MAX_UNIFORM_BUFFER_BINDINGS;
    uint32_t TotalStorageBufferBindings = (uint32_t)GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS;
    uint32_t TotalSamplers;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (GLint *)&TotalSamplers);

    uint32_t NumUniforBufferBindings = TotalUniforBufferBindings - ReservedUniformBlocks;
    uint32_t BaseUniforBufferBindings = ReservedUniformBlocks;
    uint32_t BaseStorageBufferBindings = 0;
    uint32_t BaseSamplers = 0;
    for (uint32_t i = 0; i < NumStages; ++i)
    {
        m_BaseBindings[i].UniformBuffer = Extract(BaseUniforBufferBindings, NumUniforBufferBindings, TotalUniforBufferBindings / NumStages, LimitUniforBuffer[i]);
        m_BaseBindings[i].ShaderStorageBuffer = Extract(BaseStorageBufferBindings, TotalStorageBufferBindings, TotalStorageBufferBindings / NumStages, LimitStorageBuffer[i]);
        m_BaseBindings[i].Sampler = Extract(BaseSamplers, TotalSamplers, TotalSamplers / NumStages, LimitSamplers[i]);
    }

    uint32_t MaxImages;
    glGetIntegerv(GL_MAX_IMAGE_UNITS, (GLint *)&MaxImages);
    uint32_t BaseImages = 0;
    m_BaseBindings[ShaderType_Fragment].Image = Extract(BaseImages, MaxImages, std::max(4U, MaxImages / NumStages), LimitImages[ShaderType_Fragment]);
    uint32_t TotalExtractedImages = MaxImages / (NumStages - 1);
    for (uint32_t i = 0; i < NumStages; i++) 
    {
        if (i == ShaderType_Fragment) 
        {
            continue;
        }
        m_BaseBindings[i].Image = Extract(BaseImages, MaxImages, TotalExtractedImages, LimitImages[i]);
    }
    m_BaseBindings[ShaderType_Compute].UniformBuffer = 0;
    m_BaseBindings[ShaderType_Compute].ShaderStorageBuffer = 0;
    m_BaseBindings[ShaderType_Compute].Sampler = 0;
    m_BaseBindings[ShaderType_Compute].Image = 0;

    for (size_t i = 0, n = sizeof(LimitUniforBuffer) / sizeof(LimitUniforBuffer[0]); i < n; i++) 
    {
        GLint MaxUniformBuffer;
        glGetIntegerv(LimitUniforBuffer[i], &MaxUniformBuffer);
        m_MaxUniformBuffers[i] = (uint32_t)MaxUniformBuffer;
    }
}

void OpenGLDevice::GetExtensions() 
{
    GLint Extensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &Extensions);
    m_Extensions.reserve(Extensions);
    for (GLint i = 0; i < Extensions; i++) 
    {
        m_Extensions.push_back((const char*)glGetStringi(GL_EXTENSIONS, i));
    }
}

bool OpenGLDevice::HasExtension(const char * Extension) 
{
    for (size_t i = 0, n = m_Extensions.size(); i < n; i++)
    {
        if (_stricmp(Extension, m_Extensions[i]) == 0) 
        {
            return true;
        }
    }
    return false;
}

uint32_t OpenGLDevice::Extract(uint32_t& Base, uint32_t& Num, uint32_t Amount, GLenum Limit) 
{
    if (Num < Amount) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    uint32_t CurrentBase = Base;
    GLint LimitValue;
    glGetIntegerv(Limit, &LimitValue);
    Amount = std::min(Amount, (uint32_t)LimitValue);
    Num -= Amount;
    Base += Amount;
    return CurrentBase;
}

bool OpenGLDevice::TestPreciseBug()
{
    return !TestProgram("#version 430 core\n"
    "in vec3 coords;\n"
    "out float out_value;\n"
    "uniform sampler2DShadow tex;\n"
    "void main() {\n"
    "    precise float tmp_value = vec4(texture(tex, coords)).x;\n"
    "out_value = tmp_value;\n"
    "}");
}

bool OpenGLDevice::TestProgram(const char * Program) 
{
    GLuint Shader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &Program);
    GLint LinkStatus;
    glGetProgramiv(Shader, GL_LINK_STATUS, &LinkStatus);
    glDeleteProgram(Shader);
    return LinkStatus == GL_TRUE;
}
