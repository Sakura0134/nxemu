#pragma once
#include "Shader/ShaderIR.h"
#include <string>

class OpenGLDevice;

class OpenGLShaderConstBuffer : 
    public ShaderConstBuffer 
{
public:
    OpenGLShaderConstBuffer(uint32_t MaxOffset, bool IsIndirect, uint32_t Index);

private:
    OpenGLShaderConstBuffer();
    OpenGLShaderConstBuffer& operator=(const OpenGLShaderConstBuffer&);

    uint32_t m_Index;
};

typedef std::vector<OpenGLShaderConstBuffer> OpenGLShaderConstBufferList;

std::string OpenGLDecompileShader(const OpenGLDevice & device, const ShaderIR & ir, ShaderType stage, const char * Identifier);
