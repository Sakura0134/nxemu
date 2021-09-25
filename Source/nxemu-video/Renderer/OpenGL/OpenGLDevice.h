#pragma once
#include "GpuTypes.h"
#include "Shader\Shader.h"
#include <glad\glad.h>
#include <stdint.h>
#include <vector>

class OpenGLDevice 
{
public:
    struct BaseBindings
    {
        uint32_t UniformBuffer;
        uint32_t ShaderStorageBuffer;
        uint32_t Sampler;
        uint32_t Image;
    };

    OpenGLDevice();

    uint32_t GetMaxUniformBuffers(ShaderType Type) const { return m_MaxUniformBuffers[Type]; }
    const BaseBindings & GetBaseBindings(ShaderType Type) const { return m_BaseBindings[Type]; }
    GLint GetUniformBufferAlignment() const { return m_UniformBufferAlignment; }
    bool HasWarpIntrinsics() const { return m_HasWarpIntrinsics; }
    bool HasShaderBallot() const { return m_HasShaderBallot; }
    bool HasVertexViewportLayer() const { return m_HasVertexViewportLayer; }
    bool HasImageLoadFormatted() const { return m_HasImageLoadFormatted; }
    bool HasTextureShadowLod() const { return m_HasTextureShadowLOD; }
    bool HasVertexBufferUnifiedMemory() const { return m_HasVertexBufferUnifiedMemory; }
    bool HasPreciseBug() const { return m_HasPreciseBug; }
    bool UseAssemblyShaders() const { return m_UseAssemblyShaders; }
    bool Init(void);

private:
    OpenGLDevice(const OpenGLDevice&);
    OpenGLDevice& operator=(const OpenGLDevice&);

    void BuildBaseBindings();
    void GetExtensions();
    bool HasExtension(const char * Extension);

    static uint32_t Extract(uint32_t & Base, uint32_t & Num, uint32_t Amount, GLenum Limit);
    static bool TestPreciseBug();
    static bool TestProgram(const char * Program);

    std::vector<const char *> m_Extensions;
    uint32_t m_MaxUniformBuffers[MaxShaderTypes];
    BaseBindings m_BaseBindings[MaxShaderTypes];
    GLint m_UniformBufferAlignment;
    bool m_HasWarpIntrinsics;
    bool m_HasShaderBallot;
    bool m_HasVertexViewportLayer;
    bool m_HasImageLoadFormatted;
    bool m_HasTextureShadowLOD;
    bool m_HasVertexBufferUnifiedMemory;
    bool m_HasPreciseBug;
    bool m_UseAssemblyShaders;
};
