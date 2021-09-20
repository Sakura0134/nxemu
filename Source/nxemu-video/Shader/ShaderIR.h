#pragma once
#include "Shader.h"
#include "ShaderHeader.h"
#include "ShaderNode.h"
#include <stdint.h>
#include <map>

class CVideo;
__interface IRenderer;

class ShaderConstBuffer 
{
public:
    ShaderConstBuffer();
    ShaderConstBuffer(uint32_t MaxOffset, bool IsIndirect);

    bool IsIndirect() const { return m_IsIndirect; }
    uint32_t GetMaxOffset() const { return m_MaxOffset; }

private:
    ShaderConstBuffer& operator=(const ShaderConstBuffer&);

    uint32_t m_MaxOffset;
    bool m_IsIndirect;
};

typedef std::map<uint32_t, ShaderConstBuffer> ShaderConstBufferMap;

class ShaderIR 
{
public:
    ShaderIR(const ShaderProgramCode & ProgramCode);
    ~ShaderIR();

    const ShaderConstBufferMap & GetConstantBuffers() const { return m_UsedConstBuffers; }
    const ShaderSamplerEntryList & GetSamplers() const { return m_UsedSamplers; }

private:
    ShaderIR();
    ShaderIR(const ShaderIR&);
    ShaderIR& operator=(const ShaderIR&);

    void Decode();

    const ShaderProgramCode & m_ProgramCode;
    ShaderHeader m_Header;
    ShaderConstBufferMap m_UsedConstBuffers;
    ShaderSamplerEntryList m_UsedSamplers;
};
